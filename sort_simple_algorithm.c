/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_simple_algorithm.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 18:48:26 by dswietoc          #+#    #+#             */
/*   Updated: 2026/08/30 15:41:24 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_stack_size(t_stack *stack)
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


int	ft_find_min_position(t_stack *stack)
{
	int	min;
	int	min_pos;
	int	pos;

	min = stack->value;
	min_pos = 0;
	pos = 0;
	while (stack)
	{
		if (stack->value < min)
		{
			min = stack->value;
			min_pos = pos;
		}
		stack = stack->next;
		pos++;
	}
	return (min_pos);
}

void	ft_move_to_top(t_stack **a, t_operations *op)
{
	int	pos;
	int size;

	if (!a || !*a)
		return ;
	pos = ft_find_min_position(*a);
	size = ft_stack_size(*a);
	if (pos <= size / 2)
	{
		while (pos>0)
		{
			ft_ra(a, op);
			pos--;
		}
	}
	else
	{
		while  (pos<size)
		{
			ft_rra(a, op);
			pos++;
		}
	}
	
}

void	ft_simple_sort(t_stack **a, t_stack **b, t_operations *op)
{
	while (*a)
	{
		ft_move_to_top(a, op);
		ft_pb(a, b, op);
	}
	while (*b)
	{
		ft_pa(a, b, op);
	}
	
}

int main(void)
{
	
	return (0);
}