/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 10:55:56 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/06 13:40:10 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_check_strategy_selector(char *str, t_operations *op)
{
	if (ft_strcmp(str, "--simple") == 0)
	{
		op->strategy = STRATEGY_SIMPLE;
		op->is_bench_on = false;
	}
	else if (ft_strcmp(str, "--medium") == 0)
	{
		op->strategy = STRATEGY_MEDIUM;
		op->is_bench_on = false;
	}
	else if (ft_strcmp(str, "--complex") == 0)
	{
		op->strategy = STRATEGY_COMPLEX;
		op->is_bench_on = false;
	}
	else if (ft_strcmp(str, "--adaptive") == 0)
	{
		op->strategy = STRATEGY_ADAPTIVE;
		op->is_bench_on = false;
	}
}

void	ft_init_operations(t_operations *op)
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



/*
** Run the selected sorting algorithm.
*/
static void	ft_run_strategy(int strategy, t_stack **a, t_stack **b, t_operations *op)
{
	if (strategy == 2)
		ft_medium_algorithm(a, b, op);
}

/*
** Program entry point.
*/
int	main(int argc, char **argv)
{
	t_stack	*stack_a;
	t_stack	*stack_b;
	int		strategy;
	t_operations	op;

	ft_init_operations(&op);
	stack_a = NULL;
	stack_b = NULL;
	if (!ft_validate_input(argc, argv, &stack_a))
		ft_print_error(&stack_a, &stack_b);
	strategy = ft_check_strategy_selector(argv[1], &op);
	ft_run_strategy(strategy, &stack_a, &stack_b, &op);
	ft_free_stack(&stack_a);
	ft_free_stack(&stack_b);
	return (0);
}