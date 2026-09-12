/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 13:03:13 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/06 16:58:23 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/* Swap the first two elements at the top of a stack. */
static int	ft_swap(t_stack **stack)
{
	t_stack	*first;
	t_stack	*second;

	if (!stack || !*stack || !(*stack)->next)
		return (1);
	first = *stack;
	second = first->next;
	first->next = second->next;
	second->next = first;
	*stack = second;
	return (0);
}

/* Swap the first two elements of stack a. */
void	ft_sa(t_stack **stack_a, t_operations *op)
{
	if (ft_swap(stack_a) == 0)
	{
		op->sa++;
		op->total_ops++;
		write(1, "sa\n", 3);	
	}
}

/* Swap the first two elements of stack b. */
void	ft_sb(t_stack **stack_b, t_operations *op)
{
	if (ft_swap(stack_b) == 0)
	{
		op->sb++;
		op->total_ops++;
		write(1, "sb\n", 3);
	}
}

/* Perform sa and sb. */
void	ft_ss(t_stack **stack_a, t_stack **stack_b, t_operations *op)
{
	int swap_a;
	int swap_b;
	
	swap_a = ft_swap(stack_a);
	swap_b = ft_swap(stack_b);
	if (swap_a == 0 || swap_b == 0)
	{
		op->ss++;
		op->total_ops++;
		write(1, "ss\n", 3);
	}
}
