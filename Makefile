NAME = libftpp.a

CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++20

SRCS =	data_structures/data_buffer/data_buffer.cpp \
				design_patterns/memento/memento.cpp \
				iostream/thread_safe_iostream/thread_safe_iostream.cpp \
				threading/thread/thread.cpp \
				threading/worker_pool/worker_pool.cpp \
				threading/persistent_worker/persistent_worker.cpp \
				network/message/message.cpp \
				network/client/client.cpp \
				network/server/server.cpp \
				mathematics/random_2D_coordinate_generator/random_2D_coordinate_generator.cpp \
				mathematics/perlin_noise_2D/perlin_noise_2D.cpp \
				bonus/chronometer/chronometer.cpp \
				bonus/timer/timer.cpp

OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean fclean re

all: $(NAME)

%.o: %.cpp
	@echo "Compiling" $@
	@$(CC) $(CFLAGS) -o $@ -c $<
	
$(NAME): $(OBJS)
	@echo "Compiling" $(NAME)
	@ar rcs $(NAME) $(OBJS)

clean:
	@echo "Removing all object files"
	@rm -f $(OBJS)

fclean: clean
	@echo "Removing" $(NAME)
	@rm -f $(NAME)

re: fclean all
