/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/10 10:54:20 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/10 10:54:40 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	ft_is_space(char c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

static int	ft_add_slice(t_stack **stack, char *str, int len)
{
	char	*number;
	int		i;
	int		result;

	number = malloc(sizeof(char) * (len + 1));
	if (!number)
		return (0);
	i = 0;
	while (i < len)
	{
		number[i] = str[i];
		i++;
	}
	number[i] = '\0';
	result = 0;
	if (ft_is_int(number))
		result = ft_add_value(stack, ft_atoi(number));
	free(number);
	return (result);
}

int	ft_add_argument(t_stack **stack, char *str)
{
	int	end;
	int	start;
	int	added;

	end = 0;
	added = 0;
	while (str[end])
		end++;
	while (end > 0)
	{
		while (end > 0 && ft_is_space(str[end - 1]))
			end--;
		if (end == 0)
			break ;
		start = end;
		while (start > 0 && !ft_is_space(str[start - 1]))
			start--;
		if (!ft_add_slice(stack, str + start, end - start))
			return (0);
		added = 1;
		end = start;
	}
	return (added);
}
