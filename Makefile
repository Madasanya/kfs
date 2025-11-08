AS			= i386-nasm
ASFLAGS		= -felf32
RM			= rm -f
SRCD		= ./src/
SRC_AS		= boot.asm
SRCF_AS		= $(addprefix $(SRCD),$(SRC_AS))
OBJD		= ./obj/
#OBJ			= $(SRC:%.s=%.o)
OBJF_AS 	= $(SRCF_AS:$(SRCD)%.asm=$(OBJD)%.o)
NAME		= md_kernel.bin

$(OBJD)%.o: $(SRCD)%.asm 
			@mkdir -p $(OBJD)
			${AS} ${ASFLAGS} $< -o $@

$(NAME):	${OBJF_AS}
			${AR} ${ARFLAGS} ${NAME} ${OBJF} #as linker here

all:		${NAME}

clean:		
			${RM} ${OBJD}*.o

fclean:		clean
			${RM} ${NAME} test *.txt #add all the things to be deleted

re:			fclean all

build_gcc:
			if not sudo
				echo Needs to be called by sudo
				exit 1
			else
				bash ./sripts/build_gcc_cross_compiler.sh

build_as:
			if not sudo
				echo Needs to be called by sudo
				exit 1
			else
				bash ./sripts/build_nasm_cross_assembler.sh

build_tools:	build_gcc build_as

create_image:
			if not sudo
				echo Needs to be called by sudo
				exit 1
			else
				bash ./sripts/create_img.sh
			
run:
			create_image
			
.PHONY:		all clean fclean re test