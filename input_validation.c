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
** Check whether value already exists in the stack.
*/
int	ft_has_duplicate(t_stack *stack, int value)
{
	while (stack)
	{
		if (stack->value == value)
			return (1);
		stack = stack->next;
	}
	return (0);
}

/*
** Add one value to the end of the stack.
*/
static int	ft_add_back(t_stack **stack, int value)
{
	t_stack	*new_node;
	t_stack	*current;

	new_node = malloc(sizeof(t_stack));
	if (!new_node)
		return (0);
	new_node->value = value;
	new_node->next = NULL;
	if (!*stack)
	{
		*stack = new_node;
		return (1);
	}
	current = *stack;
	while (current->next)
		current = current->next;
	current->next = new_node;
	return (1);
}

/*
** Validate all numbers and create the initial stack_a.
*/
int	ft_create_stack(t_stack **stack_a, int argc, char **argv)
{
	int	i;
	int	value;

	i = 2;
	while (i < argc)
	{
		if (!ft_is_number(argv[i]) || !ft_is_int(argv[i]))
			return (0);
		value = (int)ft_atoi(argv[i]);
		if (ft_has_duplicate(*stack_a, value))
			return (0);
		if (!ft_add_back(stack_a, value))
			return (0);
		i++;
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
