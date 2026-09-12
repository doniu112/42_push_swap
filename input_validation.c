/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_validation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 08:50:34 by ryakubov          #+#    #+#             */
/*   Updated: 2026/09/06 12:37:40 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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

int	ft_is_int(char *str)
{
	long long	number;
	long long	limit;
	int			digit;

	if (!ft_is_number(str))
		return (0);
	limit = INT_MAX;
	if (*str == '-')
		limit = -(long long)INT_MIN;
	if (*str == '+' || *str == '-')
		str++;
	number = 0;
	while (*str)
	{
		digit = *str - '0';
		if (number > (limit - digit) / 10)
			return (0);
		number = number * 10 + digit;
		str++;
	}
	return (1);
}

/*
** Check for duplicates and add the value to the top of the stack.
*/
int	ft_add_value(t_stack **stack, int value)
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
	new_node->index = -1;
	new_node->next = *stack;
	*stack = new_node;
	return (1);
}

int	ft_create_stack(t_stack **stack_a, int argc,
		char **argv, int first_number)
{
	int	i;

	i = argc - 1;
	while (i >= first_number)
	{
		if (!ft_add_argument(stack_a, argv[i]))
			return (0);
		i--;
	}
	return (1);
}
