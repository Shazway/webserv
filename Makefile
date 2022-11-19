NAME	=	webserv

CC	=	c++
FLAGS = -Wall -Wextra -Werror -std=c++98 -g3

SRC_PATH	=	srcs/
OBJ_PATH	=	obj/
INC	=	-Iincludes

SRC_NAME	=	main.cpp						\
				trees/MethodTree.cpp			\
				trees/IndexTree.cpp				\
				trees/RedirectTree.cpp			\
				trees/utilsTree.cpp				\
				Server.cpp						\
				print_functions.cpp				\
				parsing/ParsingConfig.cpp		\
				parsing/ft_split.cpp			\
				parsing/post_parse.cpp			\
				parsing/parse_elems.cpp			\
				parsing/check_duplicates.cpp	\
				requests/HttpRequest.cpp		\
				requests/ParsingRequest.cpp		\
				Webserv.cpp						\
				Upload.cpp						\
				generate_answers.cpp			\
				naming.cpp						\
				CgiHandler.cpp					\
				graphics.cpp
OBJ_NAME	=	$(SRC_NAME:.cpp=.o)

OBJ	=	$(addprefix $(OBJ_PATH), $(OBJ_NAME))
SRC	=	$(addprefix $(SRC_PATH), $(SRC_NAME))


$(NAME):	$(OBJ)
	$(CC) $(FLAGS) $(INC) $(OBJ) -o $(NAME)
	clear
	@echo -n "Server ip = \033[36m"
	@ifconfig | grep 10.11 | awk '{ printf "%s", $$2 }; END { printf "\n" }'
	@echo -n "\033[0m"

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	mkdir -p $(@D)
	$(CC) $(FLAGS) $(INC) -MMD -c $< -o $@

-include $(OBJ:%.o=%.d)

all: $(NAME)

clean:
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re