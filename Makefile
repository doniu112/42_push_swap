# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/08/01 18:48:18 by dswietoc          #+#    #+#              #
#    Updated: 2026/09/06 13:44:50 by dswietoc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = push_swap

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = push_swap.c \
	input_validation.c \
	push_swap_utils.c \
	ft_push.c \
	ft_reverse_rotate.c \
	ft_rotate.c \
	ft_swap.c \
	disorder_metric.c \
	sort_medium_algorithm.c \
	sort_custom_adaptive_algorithm.c \
	sorting_utils.c \
	ft_bench.c \
	sort_simple_algorithm.c \
	sort_complex_algorithm.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c push_swap.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re