/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 19:11:26 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/05 11:56:39 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "./Libft/libft.h"

# include <unistd.h>
# include <stdbool.h>
# include <stdlib.h>

typedef struct s_stack
{
	int				value;
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
	float	disorder;
	bool	is_bench_on;
	bool	is_adaptive;
}	t_operations;

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

/* Simple sort */
int		ft_stack_size(t_stack *stack);
int		ft_find_min_position(t_stack *stack);
void	ft_move_to_top(t_stack **a, t_operations *op);
void	ft_simple_sort(t_stack **a, t_stack **b, t_operations *op);

/* Simple medium */
/* Simple complex */
/* Simple adaptive */
/* Bench */

/* UTILS */
double	ft_compute_disorder(t_stack *a);

/* Defines */
# define STRATEGY_ADAPTIVE 	0
# define STRATEGY_SIMPLE 	1
# define STRATEGY_MEDIUM 	2
# define STRATEGY_COMPLEX 	3

# define BENCH_MODE 		false

#endif