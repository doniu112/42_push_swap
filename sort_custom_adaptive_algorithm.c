/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_custom_adaptive_algorithm.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 16:01:24 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/06 13:34:56 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ft_adaptive_algorithm(t_stack **a, t_stack **b, t_operations *op)
{
	if (op->strategy == STRATEGY_ADAPTIVE)
	{
		if (op->disorder < 0.2)
		{
			op->strategy = STRATEGY_SIMPLE;
			ft_simple_sort(a, b, op);
		}
		if (op->disorder >= 0.2 && op->disorder < 0.5)
		{
			op->strategy = STRATEGY_MEDIUM;
		}
		if (op->disorder >= 0.5)
		{
			op->strategy = STRATEGY_COMPLEX;
		}
	}
}
