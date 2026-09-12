/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 19:11:26 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/06 16:28:39 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <stdbool.h>

typedef struct s_stack
{
	int				value;
	int				index;
	struct s_stack	*next;
}	t_stack;

typedef struct s_operations
{
	int		sa;
	int		sb;
	int		ss;
	int		pa;
	int		pb;
	int		ra;
	int		rb;
	int		rr;
	int		rra;
	int		rrb;
	int		rrr;
	int		total_ops;
	int		strategy;
	double	disorder;
	bool	is_bench_on;
	bool	is_adaptive;
}	t_operations;

typedef struct s_chunk
{
	int	start;
	int	end;
}	t_chunk;

/* operations push */
void	ft_pa(t_stack **stack_a, t_stack **stack_b, t_operations *op);
void	ft_pb(t_stack **stack_a, t_stack **stack_b, t_operations *op);

/* operations reverse rotate */
void	ft_rra(t_stack **stack_a, t_operations *op);
void	ft_rrb(t_stack **stack_b, t_operations *op);
void	ft_rrr(t_stack **stack_a, t_stack **stack_b, t_operations *op);

/* operations rotate */
void	ft_ra(t_stack **stack_a, t_operations *op);
void	ft_rb(t_stack **stack_b, t_operations *op);
void	ft_rr(t_stack **stack_a, t_stack **stack_b, t_operations *op);

/* operations swap */
void	ft_sa(t_stack **stack_a, t_operations *op);
void	ft_sb(t_stack **stack_b, t_operations *op);
void	ft_ss(t_stack **stack_a, t_stack **stack_b, t_operations *op);

/* Input validation */
int		ft_is_number(char *str);
int		ft_is_int(char *str);
int		ft_create_stack(t_stack **stack_a, int argc,
			char **argv, int first_number);
int		ft_add_value(t_stack **stack, int value);
int		ft_add_argument(t_stack **stack, char *str);

/* Simple sort */
int		ft_stack_size(t_stack *stack);
int		ft_find_min_position(t_stack *stack);
void	ft_move_to_top(t_stack **a, t_operations *op);
void	ft_simple_sort(t_stack **a, t_stack **b, t_operations *op);

/* Medium sort */
void	ft_medium_algorithm(t_stack **a, t_stack **b, t_operations *op);
int		ft_find_max_position(t_stack *stack);

/* Complex sort */
void	ft_radix_sort(t_stack **a, t_stack **b, t_operations *op);

/* Adaptive sort */
void	ft_adaptive_algorithm(t_stack **a, t_stack **b, t_operations *op);

/* Bench */
void	ft_bench(t_operations *op);

/* UTILS */
double	ft_compute_disorder(t_stack *a);
void	ft_free_stack(t_stack **stack);
void	ft_putstr_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_print_error(t_stack **stack_a, t_stack **stack_b);
void	ft_putchar_fd(char c, int fd);
int		ft_strcmp(char *s1, char *s2);
int		ft_atoi(const char *str);

/* Defines */
# define STRATEGY_ADAPTIVE 	0
# define STRATEGY_SIMPLE 	1
# define STRATEGY_MEDIUM 	2
# define STRATEGY_COMPLEX 	3

# define BENCH_MODE 		false

#endif