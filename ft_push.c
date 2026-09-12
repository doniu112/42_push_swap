/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_push.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 12:47:48 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/06 16:35:55 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/* Move the top element from stack_1 to the top of stack_2. */
static int	ft_push(t_stack **stack_1, t_stack **stack_2)
{
	t_stack	*tmp;

	if (!stack_1 || !*stack_1 || !stack_2)
		return (1);
	tmp = *stack_1;
	*stack_1 = (*stack_1)->next;
	tmp->next = *stack_2;
	*stack_2 = tmp;
	return (0);
}

/* Move the top element of stack b to the top of stack a. */
void	ft_pa(t_stack **stack_a, t_stack **stack_b, t_operations *op)
{
	if (ft_push(stack_b, stack_a) == 0)
	{
		op->pa++;
		op->total_ops++;
		write(1, "pa\n", 3);	
	}
}

/* Move the top element of stack a to the top of stack b. */
void	ft_pb(t_stack **stack_a, t_stack **stack_b, t_operations *op)
{
	if (ft_push(stack_a, stack_b) == 0)
	{
		op->pb++;
		op->total_ops++;
		write(1, "pb\n", 3);	
	}
}
