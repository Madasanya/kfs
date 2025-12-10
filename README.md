# kfs
Kernel from scratch

A minimal x86 kernel implementation featuring interrupt-driven architecture with GDT/IDT setup, system call interface (INT 0x80), user mode support, VGA text mode driver, PS/2 keyboard interrupt handling, virtual terminal management, and a circular buffer error logging system. Built from the ground up using GRUB Multiboot specification.

## Architecture Overview

### System Components

```
┌───────────────────────────────────────────────────────────────┐
│                     User Mode (Ring 3)                        │
├───────────────────────────────────────────────────────────────┤
│  • Nanoshell - Command line interpreter                       │
│  • System Call Wrappers (INT 0x80)                            │
│  • User Space Applications                                    │
└───────────────────────────────────────┬───────────────────────┘
                                        │ System Calls
┌───────────────────────────────────────▼───────────────────────┐
│                    Kernel Mode (Ring 0)                       │
├───────────────────────────────────────────────────────────────┤
│  Interrupt Management                                         │
│  • IDT - 256 interrupt vectors                                │
│  • IRQ1 - Keyboard interrupt handler                          │
│  • INT 0x80 - System call interface                           │
│  • PIC - Programmable Interrupt Controller                    │
├───────────────────────────────────────────────────────────────┤
│  Memory Management                                            │
│  • GDT - Kernel/User segment descriptors                      │
│  • TSS - Task State Segment for ring transitions              │
├───────────────────────────────────────────────────────────────┤
│  Core Services                                                │
│  • Main Loop - Event processing & screen management           │
│  • Error Logging - Circular buffer (90 entries)               │
│  • System Call Handlers - Write, Read, Screen, Color, etc.    │
└───────────────────────────────────────────────────────────────┘
                              │
         ┌────────────────────┼────────────────────┐
         │                    │                    │
┌────────▼────────┐  ┌────────▼────────┐  ┌────────▼───────┐
│  Driver Layer   │  │   Utility Tools │  │  Hardware I/O  │
├─────────────────┤  ├─────────────────┤  ├────────────────┤
│ • Screen        │  │ • String Utils  │  │ • Port I/O     │
│ • Keyboard      │  │ • Printf/Format │  │ • VGA Memory   │
│ • Cursor        │  │ • History Mgmt  │  │ • Keyboard IRQ │
│ • Colors        │  │ • Error Logging │  │ • PIC Control  │
│ • PIC           │  │ • Hexdump       │  └────────────────┘
└─────────────────┘  │ • Type Defs     │
                     └─────────────────┘
```

### Key Design Decisions

**1. Interrupt-Driven Architecture**
- Hardware interrupts enabled with proper IDT setup
- Keyboard driven by IRQ1 (no polling)
- System calls via INT 0x80 trap gate
- PIC properly configured and EOI handling

**2. User Mode Support**
- Privilege level separation (Ring 0 kernel, Ring 3 user)
- GDT with kernel and user segments
- TSS for safe privilege transitions
- System call interface for kernel services

**3. Multiple Virtual Screens**
- 5 independent virtual terminals with separate history buffers
- Each screen maintains 100 lines of scrollback history
- Dedicated color scheme per screen (customizable)
- Screen switching via Tab key

**4. Circular Buffer Architecture**
- Error log: 90 entries with 77-char messages
- Keyboard input: 32-char and 32-command circular buffers
- History: 100 lines per screen (81 chars each)
- Prevents memory overflow with automatic wraparound

**5. VGA Text Mode (80x25)**
- Direct VGA memory access at 0xB8000
- Hardware cursor management
- 16 foreground/background color combinations
- Single-line header with screen identification

**6. PS/2 Keyboard Driver**
- Interrupt-driven scancode processing (IRQ1)
- Shift modifier support
- Extended scancode handling (E0 prefix)
- Special command mapping for F-keys and arrows
- Separated configuration: `keyboard_settings.c` for easy layout customization

**7. Error Logging System**
- 8-level severity (Emergency to Debug)
- Non-destructive circular buffer reads
- Kernel-style log level prefixes (KERN_ERR, etc.)
- Per-level filtering for display

**8. System Call Interface**
- INT 0x80 software interrupt
- 10+ system calls: write, read, scroll, screen/color control, hexdump, error logging
- User space wrappers for easy access
- Pointer validation for security

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
│   ├── kernel.ld                      # Linker script (1MB load, user section at 2MB)
│   ├── kernel/
│   │   ├── boot.asm                   # Multiboot entry, stack setup, BSS zeroing
│   │   ├── main/
│   │   │   ├── inc/                   # Main kernel headers (fully documented)
│   │   │   │   ├── gdt.h              # Global Descriptor Table
│   │   │   │   ├── idt.h              # Interrupt Descriptor Table
│   │   │   │   ├── kernel.h           # Global kernel state
│   │   │   │   ├── multiboot.h        # Multiboot specification
│   │   │   │   ├── start.h            # Kernel version structure
│   │   │   │   └── usermode.h         # User mode transition
│   │   │   └── src/
│   │   │       ├── gdt.c              # GDT initialization with TSS
│   │   │       ├── gdt_flush.asm      # GDT loading, segment register setup
│   │   │       ├── idt.c              # IDT setup and interrupt registration
│   │   │       ├── idt_flush.asm      # IDT loading via LIDT
│   │   │       ├── kernel.c           # Main event loop, screen management
│   │   │       ├── start.c            # Multiboot entry, initialization
│   │   │       └── usermode.asm       # Ring 3 transition via IRET
│   │   ├── drivers/
│   │   │   ├── inc/                   # Driver headers (fully documented)
│   │   │   │   ├── cursor.h           # Hardware cursor control
│   │   │   │   ├── io.h               # Port I/O operations
│   │   │   │   ├── keyboard.h         # PS/2 keyboard driver
│   │   │   │   ├── pic.h              # Programmable Interrupt Controller
│   │   │   │   ├── screen.h           # VGA text mode driver
│   │   │   │   └── screen_settings.h  # Screen dimensions
│   │   │   └── src/
│   │   │       ├── cursor.c           # Cursor positioning via VGA registers
│   │   │       ├── io.asm             # IN/OUT port wrappers
│   │   │       ├── keyboard.c         # Scancode processing, buffer management
│   │   │       ├── keyboard_settings.c # Key mappings, command bindings
│   │   │       ├── pic.c              # PIC initialization, masking, EOI
│   │   │       └── screen.c           # Screen buffer, scrolling, history
│   │   ├── interrupts/
│   │   │   ├── inc/                   # Interrupt headers (fully documented)
│   │   │   │   ├── isr_dummy.h        # Dummy interrupt handlers
│   │   │   │   ├── isr_stub.h         # ISR entry stubs
│   │   │   │   ├── isr_syscall.h      # System call handlers
│   │   │   │   └── syscall.h          # System call numbers and modes
│   │   │   └── src/
│   │   │       ├── isr_dummy.asm      # Non-blocking/blocking dummy ISRs
│   │   │       ├── isr_stub21.asm     # IRQ1 keyboard handler stub
│   │   │       ├── isr_stub80.asm     # INT 0x80 syscall stub
│   │   │       ├── isr_syscall.c      # System call dispatcher
│   │   │       ├── isr_syscall_*.c    # Individual syscall implementations
│   │   │       └── irq1_keyboard_handler.c # Keyboard IRQ processing
│   │   └── tools/
│   │       ├── inc/                   # Utility headers (fully documented)
│   │       │   ├── bool.h             # Boolean type definition
│   │       │   ├── colors.h           # VGA color manipulation
│   │       │   ├── errlog.h           # Error logging system
│   │       │   ├── hexdump.h          # Memory dump utility
│   │       │   ├── history.h          # Command history buffer
│   │       │   ├── inttype.h          # Integer type definitions
│   │       │   ├── printk.h           # Kernel printf implementation
│   │       │   ├── str_utils.h        # String utilities
│   │       │   └── va_arg.h           # Variable argument macros
│   │       └── src/
│   │           ├── colors.c           # Color encoding/decoding
│   │           ├── errlog.c           # Circular error log buffer
│   │           ├── hexdump.c          # Memory hex viewer
│   │           ├── history.c          # Screen history management
│   │           ├── printk.c           # Formatted kernel logging
│   │           ├── str_utils.c        # String manipulation functions
│   │           └── vsnprintf.c        # Printf formatting engine
│   └── user/
│       ├── inc/                       # User mode headers (fully documented)
│       │   ├── nanoshell.h            # Shell interpreter
│       │   ├── nanoshell_builtins.h   # Built-in shell commands
│       │   ├── nanoshell_utils.h      # Shell utility functions
│       │   ├── user.h                 # User program definitions
│       │   ├── user_str_utils.h       # User string utilities
│       │   └── user_syscall.h         # User syscall wrappers
│       └── src/
│           ├── nanoshell.c            # Shell main loop, command parsing
│           ├── nanoshell_*.c          # Built-in commands (echo, screen, etc.)
│           ├── user_main.c            # User mode entry point
│           ├── user_str_utils.c       # String conversion utilities
│           ├── user_syscall.asm       # INT 0x80 invocation wrappers
│           └── user_sys*.c            # Individual syscall wrappers
├── boot/
│   └── grub/
│       └── grub.cfg                   # GRUB bootloader configuration
├── scripts/
│   ├── build_gcc_cross_compiler.sh    # GCC cross-compiler build
│   ├── build_nasm_cross_assembler.sh  # NASM assembler build
│   └── create_img.sh                  # Bootable disk image creation
└── Makefile                           # Build system automation
```

## Technical Details

### Memory Layout
- **Kernel Load Address**: 1MB physical (0x00100000)
- **User Space**: 2MB physical (0x00200000)
- **Stack Size**: 128KB (grows downward from load address)
- **VGA Text Buffer**: 0xB8000 (4KB for 80x25 characters)

### Interrupt Configuration
- **IDT**: 256 entries, 8-byte aligned
- **IRQ1 (0x21)**: Keyboard interrupt handler
- **INT 0x80**: System call trap gate (DPL=3 for user access)
- **PIC**: Master at 0x20, Slave at 0xA0, remapped to 0x20-0x2F

### Segmentation (GDT)
- **Kernel Code**: Base 0x0, Limit 4GB, Ring 0, Executable
- **Kernel Data**: Base 0x0, Limit 4GB, Ring 0, Read/Write
- **User Code**: Base 0x0, Limit 4GB, Ring 3, Executable
- **User Data**: Base 0x0, Limit 4GB, Ring 3, Read/Write
- **TSS**: For privilege level transitions

### System Calls (INT 0x80)
- **SYS_WRITE (0)**: Write to screen
- **SYS_READ (1)**: Read keyboard characters
- **SYS_COMMREAD (2)**: Read keyboard commands
- **SYS_ERRWRITE (3)**: Write to error log
- **SYS_ERRPRINT (4)**: Display error log
- **SYS_SCROLL (10)**: Scroll screen
- **SYS_SCREENSET (11)**: Switch virtual screen
- **SYS_COLORSET (12)**: Change color scheme
- **SYS_HEXDUMP (13)**: Display memory dump

### Compilation Flags
- **Assembly**: `-felf32` (32-bit ELF format)
- **C Compiler**: `-std=gnu99 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -Wall -Wextra -Werror`
- **Linker**: Static linking with libgcc, custom linker script

### Current Limitations
- No dynamic memory allocation (heap/malloc)
- No filesystem support
- No disk I/O beyond initial boot
- No network stack
- No multiprocessing/threading
- Single CPU core only
- Fixed memory layout

