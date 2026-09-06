/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 19:11:26 by dswietoc          #+#    #+#             */
/*   Updated: 2026/08/22 12:42:26 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>

typedef struct s_stack
{
	int				value;
	struct s_stack	*next;
}	t_stack;

/* Input validation */
int		ft_check_strategy_selector(char *str);
int		ft_is_number(char *str);
int		ft_is_int(char *str);
int		ft_has_duplicate(t_stack *stack, int value);
int		ft_create_stack(t_stack **stack_a, int argc, char **argv);
int		ft_validate_input(int argc, char **argv, t_stack **stack_a);

/* Existing utility functions */
long	ft_atoi(const char *str);
void	ft_free_stack(t_stack **stack);
void	ft_pustr_fd(char *s, int fd);

/* Stack operations */
void	ft_pa(t_stack **stack_a, t_stack **stack_b);
void	ft_pb(t_stack **stack_a, t_stack **stack_b);
void	ft_ra(t_stack **stack_a);
void	ft_rb(t_stack **stack_b);
void	ft_rr(t_stack **stack_a, t_stack **stack_b);
void	ft_rra(t_stack **stack_a);
void	ft_rrb(t_stack **stack_b);
void	ft_rrr(t_stack **stack_a, t_stack **stack_b);
void	ft_sa(t_stack **stack_a);
void	ft_sb(t_stack **stack_b);
void	ft_ss(t_stack **stack_a, t_stack **stack_b);

/* Sorting algorithms */
void	ft_medium_algorithm(t_stack **a, t_stack **b);

/* Disorder metric */
int		ft_compute_disorder(t_stack *a);

#endif