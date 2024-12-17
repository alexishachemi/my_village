# Variables

NAME					=	my_village

CC						=	gcc


SRC						=	main.c 										\
							utils/str.c									\
							utils/v2.c									\
							utils/registry.c							\
							world/world_factory.c						\
							asset.c										\
							world/world_asset.c							\
							prop/prop_factory.c							\
							prop/prop_handling.c						\
							prop/prop_child.c							\
							world/world_prop.c							\
							terrain.c									\
							world/world_terrain.c						\
							biome.c										\
							world/world_biome.c							\
							tile.c										\
							world/world_tile.c							\
							chunk.c										\
							world/world_chunk.c							\
							render/renderer/renderer_factory.c			\
							render/renderer/renderer_display.c			\
							render/renderer/renderer_debug.c			\
							render/renderer/renderer_camera.c			\
							render/texture.c							\
							render/draw_queue.c							\
							csp/constraint.c							\
							csp/constraints/local/adjacent_to_wall.c	\
							csp/constraints/local/reserved_space.c		\
							csp/constraints/local/adjacent_to_prop.c	\
							csp/constraints/local/in_corner.c			\
							csp/constraints/local/amount_range.c		\
							csp/constraints/local/on_ground.c			\
							csp/constraints/global/all_cells_connected.c\
							csp/object.c								\
							csp/collection.c							\
							csp/global_constraint.c						\
							csp/map/map_factory.c						\
							csp/map/map_handling.c						\
							csp/map/map_obj.c							\
							csp/map/map_dfs.c							\

TSRC					=	

SRC_ROOT				=	./src/

TSRC_ROOT				=	./tests/src/

NAME					=	my_village

CC						=	gcc

LIBS					=	raylib linked cvec

CFLAGS					=	-Wall -Wextra -Iinclude

ASANARGS				=	-fsanitize=address -g3

TESTARGS				=	-DTEST -fprofile-arcs -ftest-coverage --coverage -lcriterion

DEBUGARGS				=	-g3

BUILD_DIR				=	./build/

SRC_DIR					=	./src/

PROD_DIR				=	$(BUILD_DIR)prod/

VAL_DIR					=	$(BUILD_DIR)valgrind/

ASAN_DIR				=	$(BUILD_DIR)asan/

TEST_DIR				=	$(BUILD_DIR)tests/

DEBUG_DIR				=	$(BUILD_DIR)debug/

LIB_DIR					=	./lib/

LDFLAGS					+=	$(foreach lib,$(LIBS),-L$(LIB_DIR)$(lib)/ -l$(lib))

CFLAGS					+=	$(foreach lib,$(LIBS),-I$(LIB_DIR)$(lib)/include/)

LDFLAGS 				+=	-lm

# Generated

OBJ						=	$(addprefix $(PROD_DIR), $(SRC:.c=.o))

VALOBJ					=	$(addprefix $(VAL_DIR), $(SRC:.c=.o))

ASANOBJ					=	$(addprefix $(ASAN_DIR), $(SRC:.c=.o))

TESTOBJ					=	$(addprefix $(TEST_DIR), $(SRC:.c=.o))	\
							$(addprefix $(TEST_DIR), $(TSRC:.c=.o))

DEBUGOBJ				=	$(addprefix $(DEBUG_DIR), $(SRC:.c=.o))

# Targets

$(NAME): 			lib $(OBJ) | $(PROD_DIR)
	$(CC) -o $(PROD_DIR)$(NAME) $(OBJ) $(CFLAGS) $(LDFLAGS)
	ln -f $(PROD_DIR)$(NAME) .

all:			$(NAME)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(PROD_DIR): 		| $(BUILD_DIR)
	mkdir -p $(PROD_DIR)
	rsync -a -f"+ */" -f"- *" $(SRC_ROOT) $(PROD_DIR)

$(VAL_DIR): 		| $(BUILD_DIR)
	mkdir -p $(VAL_DIR)
	rsync -a -f"+ */" -f"- *" $(SRC_ROOT) $(VAL_DIR)

$(ASAN_DIR): 		| $(BUILD_DIR)
	mkdir -p $(ASAN_DIR)
	rsync -a -f"+ */" -f"- *" $(SRC_ROOT) $(ASAN_DIR)

$(TEST_DIR): 		| $(BUILD_DIR)
	mkdir -p $(TEST_DIR)
	rsync -a -f"+ */" -f"- *" $(SRC_ROOT) $(TEST_DIR)
	rsync -a -f"+ */" -f"- *" $(TSRC_ROOT) $(TEST_DIR)

$(DEBUG_DIR): 		| $(BUILD_DIR)
	mkdir -p $(DEBUG_DIR)
	rsync -a -f"+ */" -f"- *" $(SRC_ROOT) $(DEBUG_DIR)

$(PROD_DIR)%.o:		$(SRC_ROOT)%.c | $(PROD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS)

$(VAL_DIR)%.o:		$(SRC_ROOT)%.c | $(VAL_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) $(VALARGS)

$(ASAN_DIR)%.o:		$(SRC_ROOT)%.c | $(ASAN_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) $(ASANARGS)

$(TEST_DIR)%.o:		$(SRC_ROOT)%.c | $(TEST_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) $(TESTARGS)

$(DEBUG_DIR)%.o:		$(SRC_ROOT)%.c | $(DEBUG_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) $(LDFLAGS) $(DEBUGARGS)

$(VAL_DIR)$(NAME):	$(VALOBJ) | $(VAL_DIR)
	$(CC) -o $(VAL_DIR)$(NAME) $(VALOBJ) $(CFLAGS) $(LDFLAGS) $(VALARGS)

$(ASAN_DIR)$(NAME):	$(ASANOBJ) | $(ASAN_DIR)
	$(CC) -o $(ASAN_DIR)$(NAME) $(ASANOBJ) $(CFLAGS) $(LDFLAGS) $(ASANARGS)

$(TEST_DIR)$(NAME):	$(TESTOBJ) | $(TEST_DIR)
	$(CC) -o $(TEST_DIR)$(NAME) $(TESTOBJ) $(CFLAGS) $(LDFLAGS) $(TESTARGS)

$(DEBUG_DIR)$(NAME):	$(DEBUGOBJ) | $(DEBUG_DIR)
	$(CC) -o $(DEBUG_DIR)$(NAME) $(DEBUGOBJ) $(CFLAGS) $(LDFLAGS) $(DEBUGARGS)

lib:
	for lib in $(LIBS); do make -C $(LIB_DIR)$$lib; done

grind:				$(VAL_DIR)$(NAME)

sanitize:			$(ASAN_DIR)$(NAME)

unit_tests:			$(TEST_DIR)$(NAME)

debug:				$(DEBUG_DIR)$(NAME)

clean:
	rm -f $(TESTOBJ:.o=.gcda) $(TESTOBJ:.o=.gcno) $(TESTOBJ:.o=.gcov)
	rm -f $(OBJ)
	rm -f $(VALOBJ)
	rm -f $(ASANOBJ)
	rm -f $(TESTOBJ)

lclean:
	for lib in $(LIBS); do make -C $(LIB_DIR)$$lib fclean; done

fclean: 			clean
	rm -rf $(BUILD_DIR)
	rm -rf $(NAME)

re: 				fclean all

do_grind:			grind
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(VAL_DIR)$(NAME)

tests_run:			unit_tests
	./$(TEST_DIR)$(NAME)

.PHONY: all clean fclean re lib grind sanitize unit_tests