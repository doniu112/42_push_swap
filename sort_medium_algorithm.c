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

int	ft_find_max_position(t_stack *stack)
{
	int	max;
	int	max_pos;
	int	pos;

	if (!stack)
		return (-1);
	max = stack->value;
	max_pos = 0;
	pos = 0;
	while (stack)
	{
		if (stack->value > max)
		{
			max = stack->value;
			max_pos = pos;
		}
		stack = stack->next;
		pos++;
	}
	return (max_pos);
}

static void	ft_push_chunk(t_stack **a, t_stack **b,
		t_chunk range, t_operations *op)
{
	int	rank;
	int	pushed;
	int	half;

	pushed = 0;
	half = range.start + (range.end - range.start + 1) / 2;
	while (*a && pushed < range.end - range.start + 1)
	{
		rank = ft_get_rank(*a, *b, (*a)->value);
		if (rank >= range.start && rank <= range.end)
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

static void	ft_rotate_max(t_stack **b, t_operations *op)
{
	int	pos;
	int	size;

	if (!b || !*b)
		return ;
	pos = ft_find_max_position(*b);
	size = ft_stack_size(*b);
	if (pos <= size / 2)
	{
		while (pos > 0)
		{
			ft_rb(b, op);
			pos--;
		}
	}
	else
	{
		while (pos < size)
		{
			ft_rrb(b, op);
			pos++;
		}
	}
}

void	ft_medium_algorithm(t_stack **a, t_stack **b, t_operations *op)
{
	t_chunk	range;
	int		size;
	int		chunk;

	size = ft_stack_size(*a);
	if (size < 2)
		return ;
	chunk = 1;
	while (chunk * chunk < size)
		chunk++;
	range.start = 0;
	while (range.start < size)
	{
		range.end = range.start + chunk - 1;
		if (range.end >= size)
			range.end = size - 1;
		ft_push_chunk(a, b, range, op);
		range.start += chunk;
	}
	while (*b)
	{
		ft_rotate_max(b, op);
		ft_pa(a, b, op);
	}
}
