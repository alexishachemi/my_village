NAME	=	libcjson.a

all: $(NAME)

$(NAME):
	mkdir -p include
	git submodule update --init --recursive
	$(MAKE) -C cJSON
	cp cJSON/$(NAME) ./$(NAME)
	cp cJSON/cJSON.h ./include

clean:
	$(MAKE) -C cJSON clean

fclean: clean
	rm -f $(NAME)
	rm -f include/*

re:	fclean all

.PHONY: all clean fclean re
