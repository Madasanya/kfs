# kfs
Kernel from scratch

## Building and Running

This project uses a Makefile to setup the required toolchain, build and run the kernel.

Below are the available targets:

### Toolchain Setup

Before building the kernel, you need to set up the cross-compilation toolchain:

- `make build_tools` - Builds both the cross-compiler and assembler, then installs QEMU
- `make build_gcc` - Builds the i386 GCC cross-compiler (already called by `make build_tools`)
- `make build_as` - Builds the NASM cross-assembler (already called by `make build_tools`)

**Important Notes:**
- Run `make build_tools` once before your first build to set up the development environment
- Your user must be in the sudoers group (some operations require sudo privileges)
- **Do not run make commands as root** - run them as a regular user, and ensure the same user who runs `make build_tools` is the one who builds and runs the kernel
- After running `make build_tools`, either open a new terminal or run `source $HOME/.bashrc` to update your environment

### Running the Kernel

- `make run` - Builds the kernel, creates a bootable disk image, and launches it in QEMU

This target performs the following steps:
1. Compiles all source files
2. Links the kernel binary
3. Verifies the multiboot header
4. Creates a bootable disk image at `boot/bootdisk.img`
5. Launches QEMU with 128MB of RAM

### Build Commands

- `make all` or `make` - Compiles the kernel and generates `build/md_kernel.bin` (already called by `make run`)
- `make clean` - Removes object files from the `obj/` directory
- `make fclean` - Removes object files and the built kernel binary
- `make re` - Performs a clean rebuild (equivalent to `make fclean all`)

### Advanced Targets

- `make check_bin` - Verifies that the kernel binary has a valid multiboot header (already called by `make run`)
- `make create_image` - Creates a bootable disk image from the kernel binary (already called by `make run`)

## Requirements

- i386 cross-compiler toolchain (built via `make build_tools`)
- NASM assembler (built via `make build_tools`)
- QEMU system emulator (installed via `make build_tools`)
- GRUB utilities for multiboot verification (expected to be pre-installed on the system already)
