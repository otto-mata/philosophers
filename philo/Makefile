NAME=philo
CC=clang
CFLAGS=-Wall -Werror -Wextra -std=c89 -pedantic -pedantic-errors -ggdb -fsanitize=thread
SRCS=main.c \
	init.c \
	thread.c \
	utils.c \
	utils1.c
OBJS=$(SRCS:.c=.o)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

default: all

.PHONY: default all re fclean clean