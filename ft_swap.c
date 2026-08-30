/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 13:03:13 by dswietoc          #+#    #+#             */
/*   Updated: 2026/08/30 13:38:37 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "push_swap.h"

/* Swap the first two elements at the top of a stack. */
static void	ft_swap(t_stack **stack)
{
	t_stack	*first;
	t_stack	*second;

	if (!stack || !*stack || !(*stack)->next)
		return ;
	first = *stack;
	second = first->next;
	first->next = second->next;
	second->next = first;
	*stack = second;
}

/* Swap the first two elements of stack a. */
void	ft_sa(t_stack **stack_a, t_operations *op)
{
	ft_swap(stack_a);
	op->sa++;
	op->total_ops++;
	write(1, "sa\n", 3);

}

/* Swap the first two elements of stack b. */
void	ft_sb(t_stack **stack_b, t_operations *op)
{
	ft_swap(stack_b);
	op->sb++;
	op->total_ops++;
	write(1, "sb\n", 3);
}

/* Perform sa and sb. */
void	ft_ss(t_stack **stack_a, t_stack **stack_b, t_operations *op)
{
	ft_swap(stack_a);
	ft_swap(stack_b);
	op->ss++;
	op->total_ops++;
	write(1, "ss\n", 3);
}
