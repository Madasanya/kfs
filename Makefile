AS			= i386-nasm
CC			= i386-gcc
LD			:= $(CC)
RM			= rm -rf

# Memory configuration
QEMU_MEMORY	= 128M

ASFLAGS		= -felf32 -g3 -F dwarf
CFLAGS		= -std=gnu99 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -Wall -Wextra -Werror -g3
LDFLAGS		= -ffreestanding -nostdlib -nodefaultlibs -static -lgcc
INCFLAG		= -I

ASSRC		= boot.asm \
			  drivers/src/io.asm \
			  main/src/gdt_flush.asm \
			  main/src/usermode.asm \
			  interrupts/src/isr_dummy.asm \
			  main/src/idt_flush.asm \
			  user/src/user_syscall.asm \
			  interrupts/src/isr_stub80.asm \
			  interrupts/src/isr_stub21.asm


CSRC		= main/src/kernel.c \
			  main/src/start.c \
			  tools/src/str_utils.c \
			  drivers/src/keyboard.c \
			  drivers/src/keyboard_settings.c \
			  tools/src/errlog.c \
			  tools/src/history.c \
			  drivers/src/screen.c \
			  tools/src/colors.c \
			  tools/src/printk.c \
			  tools/src/vsnprintf.c \
			  drivers/src/cursor.c \
			  main/src/gdt.c \
			  user/src/user_main.c \
			  main/src/idt.c \
			  interrupts/src/isr_syscall.c \
			  interrupts/src/isr_syscall_errprint.c \
			  user/src/user_syserrprint.c \
			  interrupts/src/isr_syscall_errwrite.c \
			  user/src/user_syserrwrite.c \
			  interrupts/src/isr_syscall_write.c \
			  interrupts/src/isr_syscall_scroll.c \
			  interrupts/src/isr_syscall_screenset.c \
			  interrupts/src/isr_syscall_hexdump.c \
			  user/src/user_syshexdump.c \
			  tools/src/hexdump.c \
			  interrupts/src/isr_syscall_colorset.c \
			  interrupts/src/irq1_keyboard_handler.c \
			  drivers/src/pic.c \
			  user/src/nanoshell.c \
			  user/src/nanoshell_echo.c \
			  user/src/user_str_utils.c \
			  interrupts/src/isr_syscall_read.c \
			  interrupts/src/isr_syscall_commread.c \
			  user/src/user_sysread.c \
			  user/src/user_sysecommread.c \



LDSRC		= kernel.ld

SRCD		= ./src/
OBJD		= ./obj/
BUILTD		= ./build/
INCD		= main/inc tools/inc drivers/inc user/inc interrupts/inc

INCPATH		:= $(addprefix $(INCFLAG)src/,$(INCD))

ASOBJS 		:= $(ASSRC:%.asm=$(OBJD)%.o)
COBJS  		:= $(CSRC:%.c=$(OBJD)%.o)
OBJS   		:= $(ASOBJS) $(COBJS)

NAME		:= $(BUILTD)md_kernel.bin

$(OBJD)%.o: $(SRCD)%.asm 
			@mkdir -p $(@D)
			${AS} ${ASFLAGS} $< -o $@

$(OBJD)%.o: $(SRCD)%.c
			@mkdir -p $(@D)
			${CC} ${CFLAGS} $(INCPATH) -c $< -o $@

$(NAME): $(OBJS)
			@mkdir -p $(@D)
			$(LD) -T $(SRCD)$(LDSRC) $(OBJS) -o $@ $(LDFLAGS)

all:		${NAME}

clean:		
			$(RM) $(OBJD)

fclean:		clean
			$(RM) $(BUILTD)
			$(RM) ./logs/

re:			fclean all

build_gcc:
				bash ./scripts/build_gcc_cross_compiler.sh

build_as:
				bash ./scripts/build_nasm_cross_assembler.sh

build_tools:	build_gcc build_as
				sudo apt install qemu-system -y

check_bin:
				echo "Checking multiboot header in $(NAME)..."
				@if grub-file --is-x86-multiboot $(NAME) > /dev/null 2>&1; then \
					echo "multiboot confirmed"; \
				else \
					echo "ERROR: $(NAME) is NOT a valid multiboot kernel"; \
					exit 1; \
				fi


create_image: check_bin
				sudo bash ./scripts/create_img.sh run
					
run: all create_image
				echo "Launching QEMU..."
				mkdir -p ./logs
				sudo qemu-system-i386 -d int,cpu_reset,invalid_mem -D ./logs/qemu_run_$(shell date +%Y%m%d_%H%M%S).log -drive file=./boot/bootdisk.img,format=raw -m $(QEMU_MEMORY)
#
debug: fclean all create_image
				echo "Launching QEMU with GDB server on port 1234..."
				mkdir -p ./logs
				sudo qemu-system-i386 -d cpu_reset,invalid_mem -D ./logs/qemu_debug_$(shell date +%Y%m%d_%H%M%S).log -drive file=./boot/bootdisk.img,format=raw -m $(QEMU_MEMORY) -s -S -cpu 486
# -nographic -monitor none -serial none

.PHONY:		all clean fclean re build_gcc build_as build_tools check_bin create_image run debug