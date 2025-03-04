CC = gcc
CFLAGS = -Werror -Wextra -Wall

PROJECT_NAME = minishell

SRC = src/*.c

INCLUDE = -Iinclude

all : $(PROJECT_NAME)

$(PROJECT_NAME) :
	$(CC) $(CFLAGS) $(SRC) $(INCLUDE) -o $(PROJECT_NAME)

clean :
	rm -f $(PROJECT_NAME)

fclean : clean

re : fclean all

run : 
	./$(PROJECT_NAME)
