/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 10:55:56 by dswietoc          #+#    #+#             */
/*   Updated: 2026/08/22 15:48:41 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** Return the selected strategy.
**
** 1 - simple
** 2 - medium
** 3 - complex
** 4 - adaptive
** 0 - invalid strategy
*/
int	ft_check_strategy_selector(char *str)
{
	if (strcmp(str, "--simple") == 0)
		return (1);
	if (strcmp(str, "--medium") == 0)
		return (2);
	if (strcmp(str, "--complex") == 0)
		return (3);
	if (strcmp(str, "--adaptive") == 0)
		return (4);
	return (0);
}

/*
** Print the required error message and exit.
*/
static void	ft_print_error(t_stack **stack_a, t_stack **stack_b)
{
	write(2, "Error\n", 6);
	ft_free_stack(stack_a);
	ft_free_stack(stack_b);
	exit(1);
}

/*
** Run the selected sorting algorithm.
*/
static void	ft_run_strategy(int strategy, t_stack **a, t_stack **b)
{
	if (strategy == 2)
		ft_medium_algorithm(a, b);
}

/*
** Program entry point.
*/
int	main(int argc, char **argv)
{
	t_stack	*stack_a;
	t_stack	*stack_b;
	int		strategy;

	stack_a = NULL;
	stack_b = NULL;
	if (!ft_validate_input(argc, argv, &stack_a))
		ft_print_error(&stack_a, &stack_b);
	strategy = ft_check_strategy_selector(argv[1]);
	ft_run_strategy(strategy, &stack_a, &stack_b);
	ft_free_stack(&stack_a);
	ft_free_stack(&stack_b);
	return (0);
}