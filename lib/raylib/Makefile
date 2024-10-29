NAME	=	libraylib.a

all: $(NAME)

$(NAME):
	mkdir -p include
	git submodule update --init --recursive
	(cd raylib && cmake -B build && cmake --build build)
	cp raylib/build/raylib/libraylib.a ./$(NAME)
	cp raylib/build/raylib/include/*.h ./include
	cp raygui/src/raygui.h ./include

clean:
	cmake --build raylib/build --target clean

fclean: clean
	rm -f $(NAME)
	rm -f include/*

re:	fclean all

.PHONY: all clean fclean re
