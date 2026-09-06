/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex_algorithm.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 18:48:23 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/06 16:28:39 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ft_index_assign(t_stack *stack_a)
{
	int	smaller_values;
	int	value;

	while ()
	{
		if ()
	}
}

void	ft_radix_sort(t_stack *stack_a, t_stack *stack_b, t_operations *op)
{
	int	size;
	int	bit;
	int	max_bits;
	int	i;

	size = ft_stack_size(stack_a);
	max_bits = 0;
	while ((size - 1) >> max_bits)
		max_bits++;
	bit = 0;
	while (bit < max_bits)
	{
		i = 0;
		while (i < size)
		{
			if ((((stack_a)->index >> bit) & 1) == 0)
				ft_pb(stack_a, stack_b, op);
			else
				ft_ra(stack_a, op);
			i++;
		}
		while (stack_b)
			ft_pa(stack_a, stack_b, op);
		bit++;
	}
}
