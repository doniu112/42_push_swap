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

static void	ft_index_assign(t_stack *a)
{
	t_stack	*current;
	t_stack	*other;

	current = a;
	while (current)
	{
		current->index = 0;
		other = a;
		while (other)
		{
			if (other->value < current->value)
				current->index++;
			other = other->next;
		}
		current = current->next;
	}
}

static void	ft_radix_pass(t_stack **a, t_stack **b,
		t_operations *op, int bit)
{
	int	remaining;

	remaining = ft_stack_size(*a);
	while (remaining > 0)
	{
		if ((((*a)->index >> bit) & 1) == 0)
			ft_pb(a, b, op);
		else
			ft_ra(a, op);
		remaining--;
	}
	while (*b)
		ft_pa(a, b, op);
}

void	ft_radix_sort(t_stack **a, t_stack **b, t_operations *op)
{
	int	max_index;
	int	bit;

	if (!a || !b || !*a || !(*a)->next)
		return ;
	ft_index_assign(*a);
	max_index = ft_stack_size(*a) - 1;
	bit = 0;
	while (max_index > 0)
	{
		ft_radix_pass(a, b, op, bit);
		max_index >>= 1;
		bit++;
	}
}
