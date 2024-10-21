##
## EPITECH PROJECT, 2024
## CPP-ECS
## File description:
## Makefile
##

# Variables

SRC						=	main.c


SRC_DIR					=	./src/

NAME					=	a.out

CC						=	gcc

CFLAGS					=	-Wall -Wextra

VALARGS					=	-g3

ASANARGS				=	-fsanitize=address -g3

TESTARGS				=	-fprofile-arcs -ftest-coverage

BUILD_DIR				=	./build/

PROD_DIR				=	$(BUILD_DIR)prod/

VAL_DIR					=	$(BUILD_DIR)valgrind/

ASAN_DIR				=	$(BUILD_DIR)asan/

TEST_DIR				=	$(BUILD_DIR)tests/

# Generated

OBJ						=	$(addprefix $(PROD_DIR), $(SRC:.c=.o))

VALOBJ					=	$(addprefix $(VAL_DIR), $(SRC:.c=.o))

ASANOBJ					=	$(addprefix $(ASAN_DIR), $(SRC:.c=.o))

TESTOBJ					=	$(addprefix $(TEST_DIR), $(SRC:.c=.o))

# Targets

$(NAME):				$(OBJ) | $(PROD_DIR)
	$(CC) -o $(PROD_DIR)$(NAME) $(OBJ)
	ln -f $(PROD_DIR)$(NAME) .

all:					$(NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(PROD_DIR): | $(BUILD_DIR)
	mkdir -p $(PROD_DIR)
	rsync -a -f"+ */" -f"- *" $(SRC_DIR) $(PROD_DIR)

$(VAL_DIR): | $(BUILD_DIR)
	mkdir -p $(VAL_DIR)
	rsync -a -f"+ */" -f"- *" $(SRC_DIR) $(VAL_DIR)

$(ASAN_DIR): | $(BUILD_DIR)
	mkdir -p $(ASAN_DIR)
	rsync -a -f"+ */" -f"- *" $(SRC_DIR) $(ASAN_DIR)

$(TEST_DIR): | $(BUILD_DIR)
	mkdir -p $(TEST_DIR)
	rsync -a -f"+ */" -f"- *" $(SRC_DIR) $(TEST_DIR)

$(PROD_DIR)%.o:		$(SRC_DIR)%.c | $(PROD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(VAL_DIR)%.o:			$(SRC_DIR)%.c | $(VAL_DIR)
	$(CC) $(CFLAGS) $(VALARGS) -c $< -o $@

$(ASAN_DIR)%.o:			$(SRC_DIR)%.c | $(ASAN_DIR)
	$(CC) $(CFLAGS) $(ASANARGS) -c $< -o $@

$(TEST_DIR)%.o:			$(SRC_DIR)%.c | $(TEST_DIR)
	$(CC) $(CFLAGS) $(TESTARGS) -c $< -o $@

$(VAL_DIR)$(NAME):		$(VALOBJ) | $(VAL_DIR)
	$(CC) -o $(VAL_DIR)$(NAME) $(VALOBJ)

$(ASAN_DIR)$(NAME):		$(ASANOBJ) | $(ASAN_DIR)
	$(CC) -o $(ASAN_DIR)$(NAME) $(ASANOBJ) $(ASANARGS)

$(TEST_DIR)$(NAME):		$(TESTOBJ) | $(TEST_DIR)
	$(CC) -o $(TEST_DIR)$(NAME) $(TESTOBJ)

grind:				$(VAL_DIR)$(NAME)

sanitize:			$(ASAN_DIR)$(NAME)

unit_tests:			$(TEST_DIR)$(NAME)

clean:
	rm -f $(TESTOBJ:.o=.gcda) $(TESTOBJ:.o=.gcno) $(TESTOBJ:.o=.gcov)
	rm -f $(OBJ)
	rm -f $(VALOBJ)
	rm -f $(ASANOBJ)
	rm -f $(TESTOBJ)

fclean: clean
	rm -rf $(BUILD_DIR)
	rm -rf $(NAME)

re: fclean all

do_grind:		grind
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(VAL_DIR)$(NAME)

.PHONY: all clean fclean re grind sanitize unit_tests