NAME = ircserv
CC = c++
FLAGS = -Wall -Werror -Wextra -std=c++98
SRCS = main.cpp client.cpp Channel.cpp \
		$(addprefix Server/, run_and_start.cpp Server.cpp first_connexion.cpp) \

all: $(NAME)

$(NAME): $(SRCS)
		$(CC) $(FLAGS) -o $(NAME) $(SRCS)

clean:
		rm -rf *.o

fclean: clean
		rm -f $(NAME)

re: fclean all
