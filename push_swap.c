/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 10:55:56 by dswietoc          #+#    #+#             */
/*   Updated: 2026/08/30 14:25:35 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_check_strategy_selector(char *str, t_operations *op)
{
	if (strcmp(str, "--simple") == 0)
	{
		printf("Running simple algorithm\n");
		op->strategy = STRATEGY_SIMPLE;
		return (1);
	}
	else if (strcmp(str, "--medium") == 0)
	{
		printf("Running medium algorithm\n");
		op->strategy = STRATEGY_MEDIUM;
		return (1);
	}
	else if (strcmp(str, "--complex") == 0)
	{
		printf("Running complex algorithm\n");
		op->strategy = STRATEGY_COMPLEX;
		return (1);
	}
	else if (strcmp(str, "--adaptive") == 0)
	{
		printf("Running adaptive algorithm\n");
		op->strategy = STRATEGY_ADAPTIVE;
		return (1);
	}
}

void	init_operations(t_operations *op)
{
	op->sa = 0;
	op->sb = 0;
	op->ss = 0;
	op->pa = 0;
	op->pb = 0;
	op->ra = 0;
	op->rb = 0;
	op->rr = 0;
	op->rra = 0;
	op->rrb = 0;
	op->rrr = 0;
	op->total_ops = 0;
	op->disorder = 0;
	op->strategy = STRATEGY_ADAPTIVE;
	op->is_adaptive = true;
	op->is_bench_on = BENCH_MODE;
}

int	ft_push_swap(int *argc, char **argv)
{
	t_stack	*stack_a;
	t_stack	*stack_b;

}
