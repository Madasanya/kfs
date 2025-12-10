AS			= i386-nasm
CC			= i386-gcc
LD			:= $(CC)
RM			= rm -rf

# Memory configuration
QEMU_MEMORY	= 128M

ASFLAGS		= -felf32 -g3 -F dwarf
CFLAGS		= -std=gnu99 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -nodefaultlibs -Wall -Wextra -Werror -g3
CFLAGS_USER	= $(CFLAGS) -fPIC
LDFLAGS		= -ffreestanding -nostdlib -nodefaultlibs -static -lgcc
INCFLAG		= -I

ASSRCK		= boot.asm \
			  drivers/src/io.asm \
			  main/src/gdt_flush.asm \
			  main/src/usermode.asm \
			  interrupts/src/isr_dummy.asm \
			  main/src/idt_flush.asm \
			  interrupts/src/isr_stub80.asm \
			  interrupts/src/isr_stub21.asm

ASSRCU		= src/user_syscall.asm \

CSRCK		= main/src/kernel.c \
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
			  main/src/idt.c \
			  interrupts/src/isr_syscall.c \
			  interrupts/src/isr_syscall_errprint.c \
			  interrupts/src/isr_syscall_errwrite.c \
			  interrupts/src/isr_syscall_write.c \
			  interrupts/src/isr_syscall_scroll.c \
			  interrupts/src/isr_syscall_screenset.c \
			  interrupts/src/isr_syscall_hexdump.c \
			  tools/src/hexdump.c \
			  interrupts/src/isr_syscall_colorset.c \
			  interrupts/src/irq1_keyboard_handler.c \
			  drivers/src/pic.c \
			  interrupts/src/isr_syscall_read.c \
			  interrupts/src/isr_syscall_commread.c \

CSRCU		= src/user_main.c \
			  src/user_syserrprint.c \
			  src/user_syserrwrite.c \
			  src/user_syshexdump.c \
			  src/nanoshell.c \
			  src/nanoshell_echo.c \
			  src/user_str_utils.c \
			  src/user_sysread.c \
			  src/user_syscommread.c \
			  src/user_syswrite.c \
			  src/nanoshell_errorlog_write.c \
			  src/nanoshell_errorlog_print.c \
			  src/nanoshell_screen_switch.c \
			  src/user_sysscreenset.c \
			  src/nanoshell_color_switch.c \
			  src/user_syscolorset.c \
			  src/nanoshell_hexdump.c \
			  src/nanoshell_gnl.c \
			  src/nanoshell_gnc.c \
			  src/nanoshell_scroll.c \
			  src/user_sysscroll.c \
			  src/nanoshell_kernel_stack_print.c

LDSRC		= kernel.ld

SRCD		= ./src/
SRCDK		= ./src/kernel/
SRCDU		= ./src/user/

OBJD		= ./obj/
BUILTD		= ./build/
INCD		= kernel/main/inc kernel/tools/inc kernel/drivers/inc user/inc kernel/interrupts/inc

INCPATH		:= $(addprefix $(INCFLAG)src/,$(INCD))

ASOBJSK 		:= $(ASSRCK:%.asm=$(OBJD)%.o)
ASOBJSU 		:= $(ASSRCU:%.asm=$(OBJD)%.o)
COBJSU  		:= $(CSRCU:%.c=$(OBJD)%.o)
COBJSK  		:= $(CSRCK:%.c=$(OBJD)%.o)
OBJS   		:= $(ASOBJSK) $(ASOBJSU) $(COBJSK) $(COBJSU)

NAME		:= $(BUILTD)md_kernel.bin

$(ASOBJSK): $(OBJD)%.o: $(SRCDK)%.asm 
			@mkdir -p $(@D)
			${AS} ${ASFLAGS} $< -o $@

$(ASOBJSU): $(OBJD)%.o: $(SRCDU)%.asm 
			@mkdir -p $(@D)
			${AS} ${ASFLAGS} $< -o $@

$(COBJSK): $(OBJD)%.o: $(SRCDK)%.c
			@mkdir -p $(@D)
			${CC} ${CFLAGS} $(INCPATH) -c $< -o $@

$(COBJSU): $(OBJD)%.o: $(SRCDU)%.c
			@mkdir -p $(@D)
			${CC} ${CFLAGS_USER} $(INCPATH) -c $< -o $@

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
				sudo qemu-system-i386 -d cpu_reset,invalid_mem -D ./logs/qemu_run_$(shell date +%Y%m%d_%H%M%S).log -drive file=./boot/bootdisk.img,format=raw -m $(QEMU_MEMORY)
#
debug: fclean all create_image
				echo "Launching QEMU with GDB server on port 1234..."
				mkdir -p ./logs
				sudo qemu-system-i386 -d cpu_reset -D ./logs/qemu_debug_$(shell date +%Y%m%d_%H%M%S).log -drive file=./boot/bootdisk.img,format=raw -m $(QEMU_MEMORY) -s -S -cpu 486
# -nographic -monitor none -serial none

.PHONY:		all clean fclean re build_gcc build_as build_tools check_bin create_image run debug