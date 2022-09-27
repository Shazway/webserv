NAME	=	webserv

CC	=	c++
FLAGS = -Wall -Wextra -Werror -std=c++98

SRC_PATH	=	srcs/
OBJ_PATH	=	obj/
INC	=	-Iincludes

SRC_NAME	=	test_main.cpp			\
				Server.cpp				\
				Routes.cpp
OBJ_NAME	=	$(SRC_NAME:.cpp=.o)

OBJ	=	$(addprefix $(OBJ_PATH), $(OBJ_NAME))
SRC	=	$(addprefix $(SRC_PATH), $(SRC_NAME))


$(NAME):	$(OBJ)
	$(CC) $(FLAGS) $(INC) $(OBJ) -o $(NAME)

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