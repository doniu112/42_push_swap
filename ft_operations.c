/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_operations.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/27 09:50:49 by dswietoc          #+#    #+#             */
/*   Updated: 2026/08/27 09:50:49 by dswietoc         ###   ########.fr       */
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
void	ft_sa(t_stack **stack_a)
{
	ft_swap(stack_a);
	write(1, "sa\n", 3);
}

/* Swap the first two elements of stack b. */
void	ft_sb(t_stack **stack_b)
{
	ft_swap(stack_b);
	write(1, "sb\n", 3);
}

/* Perform sa and sb. */
void	ft_ss(t_stack **stack_a, t_stack **stack_b)
{
	ft_swap(stack_a);
	ft_swap(stack_b);
	write(1, "ss\n", 3);
}

/* Move the top element from stack_1 to the top of stack_2. */
void	ft_push(t_stack **stack_1, t_stack **stack_2)
{
	t_stack	*tmp;

	if (!stack_1 || !*stack_1 || !stack_2)
		return ;
	tmp = *stack_1;
	*stack_1 = (*stack_1)->next;
	tmp->next = *stack_2;
	*stack_2 = tmp;
}

/* Move the top element of stack b to the top of stack a. */
void	ft_pa(t_stack **stack_a, t_stack **stack_b)
{
	ft_push(stack_b, stack_a);
	write(1, "pa\n", 3);
}

/* Move the top element of stack a to the top of stack b. */
void	ft_pb(t_stack **stack_a, t_stack **stack_b)
{
	ft_push(stack_a, stack_b);
	write(1, "pb\n", 3);
}

/* Move the first element to the bottom of the stack. */
void	ft_rotate(t_stack **stack)
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
void	ft_ra(t_stack **stack_a)
{
	ft_rotate(stack_a);
	write(1, "ra\n", 3);
}

/* Rotate stack b upwards by one position. */
void	ft_rb(t_stack **stack_b)
{
	ft_rotate(stack_b);
	write(1, "rb\n", 3);
}

/* Perform ra and rb without printing their individual names. */
void	ft_rr(t_stack **stack_a, t_stack **stack_b)
{
	ft_rotate(stack_a);
	ft_rotate(stack_b);
	write(1, "rr\n", 3);
}

/* Move the last element to the top of the stack. */
void	ft_reverse_rotate(t_stack **stack)
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
void	ft_rra(t_stack **stack_a)
{
	ft_reverse_rotate(stack_a);
	write(1, "rra\n", 4);
}

/* Reverse rotate stack b by one position. */
void	ft_rrb(t_stack **stack_b)
{
	ft_reverse_rotate(stack_b);
	write(1, "rrb\n", 4);
}

/* Perform rra and rrb without printing their individual names. */
void	ft_rrr(t_stack **stack_a, t_stack **stack_b)
{
	ft_reverse_rotate(stack_a);
	ft_reverse_rotate(stack_b);
	write(1, "rrr\n", 3);
}