NAME = ircserv
CC = c++
FLAGS = -Wall -Werror -Wextra -std=c++98
LIBS = -lcurl -lstdc++
SRCS = client.cpp Channel.cpp main.cpp parser.cpp replies.cpp \
		$(addprefix Commands/, commands.cpp join.cpp kick.cpp mode.cpp nick_and_user.cpp privmsg.cpp topic.cpp ) \
		$(addprefix Server/, run_and_start.cpp Server.cpp first_connexion.cpp) \
		$(addprefix bot_weather/, bot_print.cpp bot_weather.cpp) \

all: $(NAME)

$(NAME): $(SRCS)
		$(CC) $(FLAGS) -o $(NAME) $(SRCS) $(LIBS)

clean:
		rm -rf *.o

fclean: clean
		rm -f $(NAME)

re: fclean all
