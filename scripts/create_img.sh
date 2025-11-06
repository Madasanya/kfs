#!/bin/bash

# Set variables
PROJ_PATH=".."
IMAGE_NAME="bootdisk"
IMAGE_FILE="$PROJ_PATH/boot/$IMAGE_NAME.img"
IMAGE_SIZE_MB=8
MOUNT_PATH="../mnt/$IMAGE_NAME"
CREATED_NEW="no"
LOOP_DEV=""
MOUNTED_NEW="no"

# Paths for files to copy (adjust these as needed)
GRUB_CONFIG_SRC="$PROJ_PATH/boot/grub/grub.cfg"  # Source path to your GRUB config file
GRUB_CONFIG_DEST="$MOUNT_PATH/boot/grub/grub.cfg"  # Destination path
KERNEL_SRC="$PROJ_PATH/build/kernel.bin"  # Source path to your kernel binary file
KERNEL_DEST="$MOUNT_PATH/boot/kernel.bin"  # Destination path (adjust name/path if needed)

# Check for mode argument
if [ $# -ne 1 ]; then
  echo "Usage: $0 <mode> (run or dev)"
  exit 1
fi
MODE="$1"
if [ "$MODE" != "run" ] && [ "$MODE" != "dev" ]; then
  echo "Invalid mode: $MODE. Must be 'run' or 'dev'."
  exit 1
fi

# Check if the image already exists
if [ -f "$IMAGE_FILE" ]; then
  echo "Image file '$IMAGE_FILE' already exists. Skipping creation, partitioning, and FAT16 setup."
else
  # Create the blank image file using qemu-img
  qemu-img create -f raw "$IMAGE_FILE" "${IMAGE_SIZE_MB}M"
  echo "Image created with qemu-img."
  CREATED_NEW="yes"
fi

# Check if the image is already set up as a loop device
LOOP_DEV=$(losetup -j "$IMAGE_FILE" | cut -d: -f1 | head -n1)
if [ -z "$LOOP_DEV" ]; then
  # Find a free loop device and attach with partitions enabled (-P)
  LOOP_DEV=$(losetup -fP --show "$IMAGE_FILE")
  echo "Image attached to new loop device: $LOOP_DEV"
  LOOP_SETUP_NEW="yes"
else
  echo "Image is already attached to loop device: $LOOP_DEV"
  LOOP_SETUP_NEW="no"
fi

# If newly created, perform partitioning and formatting
if [ "$CREATED_NEW" = "yes" ]; then
  # Create MBR partition table with one bootable FAT16 partition (type 6)
  echo "label: dos
2048,,6,*" | sfdisk "$LOOP_DEV"

  # Format the partition as FAT16
  mkfs.vfat -F 16 -s 1 "${LOOP_DEV}p1"
  echo "Partitioning and FAT16 formatting completed."
fi

# Find if the partition is already mounted, and get the mount point if any
MOUNT_POINT=$(findmnt -n -o TARGET "${LOOP_DEV}p1" | head -n1)
if [ -n "$MOUNT_POINT" ]; then
  echo "Partition already mounted at $MOUNT_POINT. Skipping mount and using it for GRUB check."
  MOUNT_PATH="$MOUNT_POINT"
  MOUNTED_NEW="no"
else
  mkdir -p "$MOUNT_PATH"
  mount "${LOOP_DEV}p1" "$MOUNT_PATH"
  echo "Mounted partition to $MOUNT_PATH."
  MOUNTED_NEW="yes"
fi

# Check if GRUB is already installed (e.g., by checking for boot/grub directory)
if [ -d "$MOUNT_PATH/boot/grub" ]; then
  echo "GRUB is already installed. Skipping installation."
else
  # Install GRUB for MBR/BIOS (i386-pc target, suitable for FAT16)
  grub-install --target=i386-pc --boot-directory="$MOUNT_PATH/boot" --fonts="" --themes="" --locales="" --modules="part_msdos fat" "$LOOP_DEV"
  echo "GRUB installed."
fi

# Copy GRUB config and kernel files
cp "$GRUB_CONFIG_SRC" "$GRUB_CONFIG_DEST"
echo "Copied GRUB config to $GRUB_CONFIG_DEST"
cp "$KERNEL_SRC" "$KERNEL_DEST"
echo "Copied kernel to $KERNEL_DEST"

# Optional: Verify
sfdisk -l "$LOOP_DEV"

# Handle cleanup based on mode
if [ "$MODE" = "run" ]; then
  # Automatic cleanup
  if [ "$MOUNTED_NEW" = "yes" ]; then
    umount "$MOUNT_PATH"
    rmdir "$MOUNT_PATH"
    echo "Unmounted and cleaned up mount directory."
  fi
  if [ "$LOOP_SETUP_NEW" = "yes" ]; then
    losetup -d "$LOOP_DEV"
    echo "Detached loop device: $LOOP_DEV"
  fi
elif [ "$MODE" = "dev" ]; then
  # Print manual cleanup commands
  echo "Dev mode: Cleanup not performed automatically. Run the following commands manually when done:"
  if [ "$MOUNTED_NEW" = "yes" ]; then
    echo "umount \"$MOUNT_PATH\""
    echo "rmdir \"$MOUNT_PATH\""
  fi
  if [ "$LOOP_SETUP_NEW" = "yes" ]; then
    echo "losetup -d \"$LOOP_DEV\""
  fi
  if [ "$MOUNTED_NEW" != "yes" ] && [ "$LOOP_SETUP_NEW" != "yes" ]; then
    echo "No new mounts or loop setups were performed by this run."
  fi
fi
