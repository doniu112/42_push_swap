/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_custom_adaptive_algorithm.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 16:01:24 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/06 15:05:37 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ft_adaptive_algorithm(t_stack **a, t_stack **b,
		t_operations *op)
{
	if (op->disorder < 0.2)
	{
		op->strategy = STRATEGY_SIMPLE;
		ft_simple_sort(a, b, op);
	}
	else if (op->disorder < 0.5)
	{
		op->strategy = STRATEGY_MEDIUM;
		ft_medium_algorithm(a, b, op);
	}
	else
	{
		op->strategy = STRATEGY_COMPLEX;
		ft_radix_sort(a, b, op);
	}
}
