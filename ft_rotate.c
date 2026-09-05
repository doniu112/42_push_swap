/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rotate.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 13:03:22 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/05 11:53:40 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/* Move the first element to the bottom of the stack. */
static void	ft_rotate(t_stack **stack)
{
	t_stack	*last;
	t_stack	*current;

	if (!stack || !*stack || !(*stack)->next)
		return ;
	current = *stack;
	while (current->next)
		current = current->next;
	last = current;
	current = *stack;
	*stack = (*stack)->next;
	last->next = current;
	current->next = NULL;
}

/* Rotate stack a upwards by one position. */
void	ft_ra(t_stack **stack_a, t_operations *op)
{
	ft_rotate(stack_a);
	op->ra++;
	op->total_ops++;
	write(1, "ra\n", 3);
}

/* Rotate stack b upwards by one position. */
void	ft_rb(t_stack **stack_b, t_operations *op)
{
	ft_rotate(stack_b);
	op->rb++;
	op->total_ops++;
	write(1, "rb\n", 3);
}

/* Perform ra and rb without printing their individual names. */
void	ft_rr(t_stack **stack_a, t_stack **stack_b, t_operations *op)
{
	ft_rotate(stack_a);
	ft_rotate(stack_b);
	op->rr++;
	op->total_ops++;
	write(1, "rr\n", 3);
}
