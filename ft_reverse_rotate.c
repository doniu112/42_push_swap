/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_reverse_rotate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 13:03:30 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/05 11:48:49 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/* Move the last element to the top of the stack. */
static int	ft_reverse_rotate(t_stack **stack)
{
	t_stack	*last;
	t_stack	*current;

	if (!stack || !*stack || !(*stack)->next)
		return (0);
	current = *stack;
	while (current->next && current->next->next)
		current = current->next;
	last = current->next;
	current->next = NULL;
	last->next = *stack;
	*stack = last;
	return (1);
}

/* Reverse rotate stack a by one position. */
void	ft_rra(t_stack **stack_a, t_operations *op)
{
	if (!ft_reverse_rotate(stack_a))
		return ;
	op->rra++;
	op->total_ops++;
	write(1, "rra\n", 4);
}

/* Reverse rotate stack b by one position. */
void	ft_rrb(t_stack **stack_b, t_operations *op)
{
	if (!ft_reverse_rotate(stack_b))
		return ;
	op->rrb++;
	op->total_ops++;
	write(1, "rrb\n", 4);
}

/* Perform rra and rrb without printing their individual names. */
void	ft_rrr(t_stack **stack_a, t_stack **stack_b, t_operations *op)
{
	int	moved_a;
	int	moved_b;

	moved_a = ft_reverse_rotate(stack_a);
	moved_b = ft_reverse_rotate(stack_b);
	if (!moved_a && !moved_b)
		return ;
	op->rrr++;
	op->total_ops++;
	write(1, "rrr\n", 4);
}
