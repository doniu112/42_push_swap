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
** Print the required error message.
*/
static void	ft_print_error(void)
{
	write(2, "Error\n", 6);
}

/*
** Program entry point.
**
** stack_a contains the input numbers.
** stack_b starts empty as required by the project.
*/
int	main(int argc, char **argv)
{
	t_stack	*stack_a;
	t_stack	*stack_b;

	stack_a = NULL;
	stack_b = NULL;
	if (!ft_validate_input(argc, argv, &stack_a))
	{
		ft_print_error();
		return (1);
	}
	(void)stack_b;
	ft_free_stack(&stack_a);
	return (0);
}
