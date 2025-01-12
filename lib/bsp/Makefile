# General

NAME		=	bsp

LIB			=	lib$(NAME).a

INC			=	include/bsp.h

CC			=	gcc

TMPDIR		=	tmp

# Dependencies

DEP_DIR		=	../

DEP_LIBS	=	linked

# Sources

SRC			=	$(shell find src -type f -name '*.c')

OBJ			=	$(SRC:%.c=$(TMPDIR)/%.o)

CFLAGS		+=	-Wall -Wextra -Iinc -Iinclude

CFLAGS		+=	$(foreach dep,$(DEP_LIBS),-I$(DEP_DIR)$(dep)/include/ -L$(DEP_DIR)$(dep)/ -l$(dep))

# Rules

$(LIB):	lib $(OBJ)
	ar rc $(LIB) $(OBJ)

all:	$(LIB)

lib:
	for dep in $(DEP_LIBS); do make -C $(DEP_DIR)$$dep; done

clean:
	rm -rf $(TMPDIR)
	rm -f $(shell find . -type f -name '*.gc*')

fclean:	clean
	rm -f $(LIB)

re:	clean all

$(TMPDIR)/%.o:	%.c
	@mkdir -p $(@D)
	gcc -o $@ -c $< $(CFLAGS)

.PHONY: all lib clean fclean re
