/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_reverse_rotate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 13:03:30 by dswietoc          #+#    #+#             */
/*   Updated: 2026/08/30 13:38:37 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "push_swap.h"

/* Move the last element to the top of the stack. */
static void	ft_reverse_rotate(t_stack **stack)
{
	t_stack	*last;
	t_stack	*current;

	if (!stack || !*stack || !(*stack)->next)
		return ;
	current = *stack;
	while (current->next && current->next->next)
		current = current->next;
	last = current->next;
	current->next = NULL;
	last->next = *stack;
	*stack = last;
}

/* Reverse rotate stack a by one position. */
void	ft_rra(t_stack **stack_a, t_operations *op)
{
	ft_reverse_rotate(stack_a);
	op->rra++;
	op->total_ops++;
	write(1, "rra\n", 4);
}

/* Reverse rotate stack b by one position. */
void	ft_rrb(t_stack **stack_b, t_operations *op)
{
	ft_reverse_rotate(stack_b);
	op->rrb++;
	op->total_ops++;
	write(1, "rrb\n", 4);
}

/* Perform rra and rrb without printing their individual names. */
void	ft_rrr(t_stack **stack_a, t_stack **stack_b, t_operations *op)
{
	ft_reverse_rotate(stack_a);
	ft_reverse_rotate(stack_b);
	op->rrr++;
	op->total_ops++;
	write(1, "rrr\n", 3);
}