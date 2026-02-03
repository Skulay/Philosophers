NAME	= philo

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -pthread -g3
HEADER  = philo.h
SRCS	= main.c \
			parsing.c \
			philo.c \
			time_sleep.c \
			init.c \
			monitoring.c \
			routine.c \
			utils.c \
			free_destroy.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
