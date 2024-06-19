NAME=thread_pool

CC=gcc
CFLAGS=-Wall -Wextra -O3 -lpthread -fsanitize=leak

SRC=thread_pool.c queue.c test.c

DEBUG=debug


all: $(NAME)


$(NAME): clean
	$(CC) $(CFLAGS) $(SRC) -o $@


$(DEBUG): clean
	$(CC) $(CFLAGS) -g $(SRC) -o $@


clean:
	rm -f $(NAME) $(DEBUG)
