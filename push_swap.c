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

static int	ft_get_strategy(char *arg)
{
	if (ft_strcmp(arg, "--simple") == 0)
		return (STRATEGY_SIMPLE);
	if (ft_strcmp(arg, "--medium") == 0)
		return (STRATEGY_MEDIUM);
	if (ft_strcmp(arg, "--complex") == 0)
		return (STRATEGY_COMPLEX);
	if (ft_strcmp(arg, "--adaptive") == 0)
		return (STRATEGY_ADAPTIVE);
	return (-1);
}

int	ft_parse_options(int argc, char **argv,
		t_operations *op, int *first_number)
{
	int	i;
	int	strategy;
	int	strategy_seen;

	i = 0;
	strategy_seen = 0;
	while (++i < argc)
	{
		strategy = ft_get_strategy(argv[i]);
		if (ft_strcmp(argv[i], "--bench") == 0)
			op->is_bench_on = true;
		else if (strategy != -1)
		{
			if (strategy_seen)
				return (0);
			strategy_seen = 1;
			op->strategy = strategy;
			op->is_adaptive = (strategy == STRATEGY_ADAPTIVE);
		}
		else
			break ;
	}
	*first_number = i;
	return (1);
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
static void	ft_run_strategy(t_stack **a, t_stack **b,
		t_operations *op)
{
	if (op->strategy == STRATEGY_SIMPLE)
		ft_simple_sort(a, b, op);
	else if (op->strategy == STRATEGY_MEDIUM)
		ft_medium_algorithm(a, b, op);
	else if (op->strategy == STRATEGY_COMPLEX)
		ft_radix_sort(a, b, op);
	else
		ft_adaptive_algorithm(a, b, op);
}

/*
** Program entry point.
*/
int	main(int argc, char **argv)
{
	t_stack			*stack_a;
	t_stack			*stack_b;
	t_operations	op;
	int				first_number;

	if (argc == 1)
		return (0);
	stack_a = NULL;
	stack_b = NULL;
	ft_init_operations(&op);
	first_number = 1;
	if (!ft_parse_options(argc, argv, &op, &first_number)
		|| first_number == argc
		|| !ft_create_stack(&stack_a, argc, argv, first_number))
		ft_print_error(&stack_a, &stack_b);
	op.disorder = ft_compute_disorder(stack_a);
	if (op.is_adaptive && op.disorder == 0.0)
		op.strategy = STRATEGY_SIMPLE;
	if (op.disorder != 0.0)
		ft_run_strategy(&stack_a, &stack_b, &op);
	if (op.is_bench_on)
		ft_bench(&op);
	ft_free_stack(&stack_a);
	ft_free_stack(&stack_b);
	return (0);
}
