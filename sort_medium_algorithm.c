/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_medium_algorithm.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 10:21:47 by ryakubov          #+#    #+#             */
/*   Updated: 2026/09/06 15:29:57 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** Return the number of elements in the stack.
*/
static int	ft_stack_size(t_stack *stack)
{
	int	size;

	size = 0;
	while (stack)
	{
		size++;
		stack = stack->next;
	}
	return (size);
}

/*
** Return the zero-based rank of a value in both stacks.
*/
static int	ft_get_rank(t_stack *a, t_stack *b, int value)
{
	int	rank;

	rank = 0;
	while (a)
	{
		if (a->value < value)
			rank++;
		a = a->next;
	}
	while (b)
	{
		if (b->value < value)
			rank++;
		b = b->next;
	}
	return (rank);
}

/*
** Push one chunk from stack a to stack b.
*/
static void	ft_push_chunk(t_stack **a, t_stack **b,
		int start, int end, t_operations *op)
{
	int	rank;
	int	pushed;
	int	half;

	pushed = 0;
	half = start + (end - start + 1) / 2;
	while (*a && pushed < end - start + 1)
	{
		rank = ft_get_rank(*a, *b, (*a)->value);
		if (rank >= start && rank <= end)
		{
			ft_pb(a, b, op);
			pushed++;
			if (rank < half)
				ft_rb(b, op);
		}
		else
			ft_ra(a, op);
	}
}

/*
** Move the largest element of stack b to the top.
*/
static void	ft_rotate_max(t_stack **a, t_stack **b, t_operations *op)
{
	int	rank;
	int	size;

	size = ft_stack_size(*b);
	while (*b)
	{
		rank = ft_get_rank(*a, *b, (*b)->value);
		if (rank == size - 1)
			break ;
		ft_rb(b, op);
	}
}

/*
** Sort stack a using sqrt(n) value chunks.
*/
void	ft_medium_algorithm(t_stack **a, t_stack **b, t_operations *op)
{
	int	size;
	int	chunk;
	int	start;
	int	end;
	op->disorder = ft_compute_disorder(a);
	size = ft_stack_size(*a);
	if (size < 2)
		return ;
	chunk = 1;
	while (chunk * chunk < size)
		chunk++;
	start = 0;
	while (start < size)
	{
		end = start + chunk - 1;
		if (end >= size)
			end = size - 1;
		ft_push_chunk(a, b, start, end, op);
		start += chunk;
	}
	while (*b)
	{
		ft_rotate_max(a, b, op);
		ft_pa(a, b, op);
	}
}
