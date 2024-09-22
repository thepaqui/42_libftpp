NAME = libftpp.a

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++2a

SRCS = 

MANDATORY_OBJS = $(SRCS:.c=.o)

BONUS_SRCS = 

BONUS_OBJS = $(BONUS_SRCS:.c=.o)

ifndef WITH_BONUS
OBJS = $(MANDATORY_OBJS)
else
OBJS = $(MANDATORY_OBJS) $(BONUS_OBJS)
endif

.PHONY: all clean fclean re bonus

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
	
$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean:
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: 
	make WITH_BONUS=1 all
