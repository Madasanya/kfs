AS			= i386-nasm
CC			= i386-gcc
LD			:= $(CC)
RM			= rm -rf

ASFLAGS		= -felf32
CFLAGS		= -std=gnu99 -ffreestanding -Wall -Wextra
LDFLAGS		= -ffreestanding -nostdlib -lgcc
INCFLAG		= -I

ASSRC		= boot.asm
CSRC		= main/src/kernel.c main/src/start.c tools/src/str_utils.c  tools/src/errlog.c tools/src/printk.c tools/src/vsnprintf.c
LDSRC		= kernel.ld

SRCD		= ./src/
OBJD		= ./obj/
BUILTD		= ./build/
INCD		= main/inc tools/inc

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

re:			fclean all

build_gcc:
				bash ./scripts/build_gcc_cross_compiler.sh

build_as:
				bash ./scripts/build_nasm_cross_assembler.sh

build_tools:	build_gcc build_as
				source $HOME/.bashrc
				sudo apt install qemu-system

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
				sudo qemu-system-i386 -drive file=./boot/bootdisk.img,format=raw -m 128M
				

.PHONY:		all clean fclean re build_gcc build_as build_tools check_bin create_image run