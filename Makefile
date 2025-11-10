AS			= i386-nasm
CC			= i386-gcc
LD			:= $(CC)
RM			= rm -rf

ASFLAGS		= -felf32
CFLAGS		= -std=gnu99 -ffreestanding -Wall -Wextra
LDFLAGS		= -ffreestanding -O2 -nostdlib -lgcc
INCFLAG		= -I

ASSRC		= boot.asm
CSRC		= main/src/kernel.c main/src/start.c tools/src/str_utils.c
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
			@if [ "$$(id -u)" -ne 0 ]; then \
				printf "\033[31mERROR:\033[0m Needs to be called with sudo (run: sudo make $@)\n"; \
				exit 1; \
			fi
				bash ./sripts/build_gcc_cross_compiler.sh

build_as:
			@if [ "$$(id -u)" -ne 0 ]; then \
				printf "\033[31mERROR:\033[0m Needs to be called with sudo (run: sudo make $@)\n"; \
				exit 1; \
			fi
				bash ./sripts/build_nasm_cross_assembler.sh

build_tools:	build_gcc build_as

create_image:
			@if [ "$$(id -u)" -ne 0 ]; then \
				printf "\033[31mERROR:\033[0m Needs to be called with sudo (run: sudo make $@)\n"; \
				exit 1; \
			fi
			bash ./scripts/create_img.sh
			
run:
			if grub-file --is-x86-multiboot myos.bin; then
				echo multiboot confirmed
			else
				echo the file is not multiboot
			fi
			create_image
			qemu-system-i386 -drive file=bootdisk.img,format=raw -m 128M 

.PHONY:		all clean fclean re test