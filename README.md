# kfs
Kernel from scratch

A minimal x86 kernel implementation featuring a custom VGA text mode driver, PS/2 keyboard support, virtual terminal management, and a circular buffer error logging system. Built from the ground up using GRUB Multiboot specification.

## Architecture Overview

### System Components

```
┌─────────────────────────────────────────────────────────────┐
│                        Kernel Layer                         │
├─────────────────────────────────────────────────────────────┤
│  • Boot (boot.asm) - Multiboot entry point                  │
│  • Main Loop (kernel.c) - Event processing & screen mgmt    │
│  • Error Logging (errlog.c) - Circular buffer logging       │
└─────────────────────────────────────────────────────────────┘
                              │
         ┌────────────────────┼────────────────────┐
         │                    │                    │
┌────────▼────────┐  ┌────────▼────────┐  ┌───────▼────────┐
│  Driver Layer   │  │      Tools      │  │  Hardware I/O  │
├─────────────────┤  ├─────────────────┤  ├────────────────┤
│ • Screen        │  │ • String Utils  │  │ • Port I/O     │
│ • Keyboard      │  │ • Printf/Format │  │ • VGA Memory   │
│ • Cursor        │  │ • History Mgmt  │  │ • PS/2 Ports   │
│ • Colors        │  │ • Type Defs     │  └────────────────┘
└─────────────────┘  └─────────────────┘
```

### Key Design Decisions

**1. Multiple Virtual Screens**
- 5 independent virtual terminals with separate history buffers
- Each screen maintains 100 lines of scrollback history
- Dedicated color scheme per screen (customizable)
- Screen switching via Tab key

**2. Circular Buffer Architecture**
- Error log: 90 entries with 77-char messages
- Keyboard input: 32-char and 32-command circular buffers
- History: 100 lines per screen (81 chars each)
- Prevents memory overflow with automatic wraparound

**3. VGA Text Mode (80x25)**
- Direct VGA memory access at 0xB8000
- Hardware cursor management
- 16 foreground/background color combinations
- Single-line header with screen identification

**4. PS/2 Keyboard Driver**
- Polling-based scancode processing (Set 1)
- Shift modifier support
- Extended scancode handling (E0 prefix)
- Special command mapping for F-keys and arrows
- Separated configuration: `keyboard_settings.c` for easy layout customization

**5. Error Logging System**
- 8-level severity (Emergency to Debug)
- Non-destructive circular buffer reads
- Kernel-style log level prefixes (KERN_ERR, etc.)
- Per-level filtering for display

## Keyboard Shortcuts

### Screen Management
- **Tab** - Switch to next virtual screen (cycles through 5 screens)
- **Right Alt** - Change color scheme for current screen
- **Up Arrow** - Scroll up through history
- **Down Arrow** - Scroll down through history

### Error Log Display
- **F1** - Display Emergency level errors (most critical)
- **F2** - Display Alert level errors
- **F3** - Display Critical level errors
- **F4** - Display Error level messages
- **F5** - Display Warning level messages
- **F6** - Display Notice level messages
- **F7** - Display Info level messages
- **F8** - Display Debug level messages (all messages)

### Text Input
- **Enter** - New line
- **Shift + Key** - Uppercase/symbols (standard layout)
- **All printable keys** - ASCII input with echo

### Visual Feedback
- **Thin cursor** (underscore) - Active input mode
- **Block cursor** - History browsing mode

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

## Project Structure

```
kfs/
├── src/
│   ├── boot.asm                       # Multiboot entry point, stack setup
│   ├── kernel.ld                      # Linker script (loads at 1MB)
│   ├── main/
│   │   ├── inc/
│   │   │   ├── kernel.h               # Main kernel header
│   │   │   ├── multiboot.h            # Multiboot specification
│   │   │   └── start.h                # Kernel version and globals
│   │   └── src/
│   │       ├── kernel.c               # Main loop and screen management
│   │       └── start.c                # Multiboot validation entry
│   ├── drivers/
│   │   ├── inc/
│   │   │   ├── cursor.h               # Hardware cursor control
│   │   │   ├── io.h                   # Port I/O operations
│   │   │   ├── keyboard.h             # PS/2 keyboard driver
│   │   │   ├── screen.h               # VGA text mode driver
│   │   │   └── screen_settings.h      # Screen dimensions
│   │   └── src/
│   │       ├── cursor.c               # Cursor positioning
│   │       ├── io.asm                 # Assembly port I/O wrappers
│   │       ├── keyboard.c             # Scancode processing logic
│   │       ├── keyboard_settings.c    # Keyboard layout & key mappings
│   │       └── screen.c               # Screen buffer management
│   └── tools/
│       ├── inc/
│       │   ├── bool.h                 # Boolean type definition
│       │   ├── colors.h               # VGA color manipulation
│       │   ├── errlog.h               # Error logging system
│       │   ├── history.h              # Command history buffer
│       │   ├── inttype.h              # Integer type definitions
│       │   ├── printk.h               # Kernel printf implementation
│       │   ├── str_utils.h            # String utilities
│       │   └── va_arg.h               # Variable arguments
│       └── src/
│           ├── colors.c               # Color encoding/decoding
│           ├── errlog.c               # Circular error log buffer
│           ├── history.c              # Screen history management
│           ├── printk.c               # Formatted logging output
│           ├── str_utils.c            # String manipulation
│           └── vsnprintf.c            # Printf formatting engine
├── boot/
│   └── grub/
│       └── grub.cfg                   # GRUB bootloader config
├── scripts/
│   ├── build_gcc_cross_compiler.sh    # GCC cross-compiler build script
│   ├── build_nasm_cross_assembler.sh  # NASM assembler build script
│   └── create_img.sh                  # Disk image creation
└── Makefile                           # Build system
```

## Technical Details

### Memory Layout
- **Kernel Load Address**: 1MB physical (0x00100000)
- **Stack Size**: 128KB
- **VGA Text Buffer**: 0xB8000 (80x25 characters)

### Compilation Flags
- **Assembly**: `-felf32` (32-bit ELF format)
- **C Compiler**: `-std=gnu99 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -Wall -Wextra -Werror`
- **Linker**: Static linking with libgcc

### Current Limitations
- No interrupt handling (interrupts disabled)
- Polling-based keyboard input
- No filesystem support
- No memory management beyond static allocation
- No multiprocessing/threading
- Single CPU core only
