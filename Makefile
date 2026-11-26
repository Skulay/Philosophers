NAME	= philo

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -pthread -g3

SRC_DIR	= src
HEADER	= $(SRC_DIR)/philo.h
SRC		= main.c \
			parsing.c \
			philo.c \
			time_sleep.c \
			init.c \
			monitoring.c \
			routine.c \
			utils.c \
			free_destroy.c

SRCS	= $(addprefix $(SRC_DIR)/, $(SRC))
OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
