/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ryakubov <ryakubov@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 08:50:34 by ryakubov          #+#    #+#             */
/*   Updated: 2026/08/30 08:50:34 by ryakubov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

/*
** Check whether the argument contains only digits,
** with an optional leading '+' or '-'.
*/
int	ft_is_number(char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/*
** Check whether the number fits into an int.
**
** The number is checked only after ft_is_number(),
** therefore ft_atoi() receives a valid numeric string.
*/
int	ft_is_int(char *str)
{
	long	value;

	value = ft_atoi(str);
	if (value < INT_MIN || value > INT_MAX)
		return (0);
	return (1);
}

/*
** Check for duplicates and add the value to the top of the stack.
*/
static int	ft_add_value(t_stack **stack, int value)
{
	t_stack	*current;
	t_stack	*new_node;

	current = *stack;
	while (current)
	{
		if (current->value == value)
			return (0);
		current = current->next;
	}
	new_node = malloc(sizeof(t_stack));
	if (!new_node)
		return (0);
	new_node->value = value;
	new_node->next = *stack;
	*stack = new_node;
	return (1);
}

/*
** Validate numbers and create stack_a in the original order.
*/
int	ft_create_stack(t_stack **stack_a, int argc, char **argv)
{
	int	i;
	int	value;

	i = argc - 1;
	while (i >= 2)
	{
		if (!ft_is_number(argv[i]) || !ft_is_int(argv[i]))
			return (0);
		value = (int)ft_atoi(argv[i]);
		if (!ft_add_value(stack_a, value))
			return (0);
		i--;
	}
	return (1);
}

/*
** Validate strategy and all input numbers.
*/
int	ft_validate_input(int argc, char **argv, t_stack **stack_a)
{
	if (argc < 3)
		return (0);
	if (ft_check_strategy_selector(argv[1]) == 0)
		return (0);
	if (!ft_create_stack(stack_a, argc, argv))
	{
		ft_free_stack(stack_a);
		return (0);
	}
	return (1);
}