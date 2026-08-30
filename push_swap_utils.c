/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 11:08:59 by dswietoc          #+#    #+#             */
/*   Updated: 2026/08/30 14:46:21 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

// void	ft_putstr_fd(char *s, int fd)
// {
// 	while (*s)
// 	{
// 		write(fd, s, 1);
// 		s++;
// 	}
// }


void	ft_free_stack(t_stack **stack)
{
	t_stack	*current;
	t_stack	*next;

	if (stack == NULL || *stack == NULL)
		return ;

	current = *stack;
	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*stack = NULL;
}

// long	ft_atoi(const char *str)
// {
// 	long	num;
// 	int		sign;

// 	num = 0;
// 	sign = 1;
// 	if (*str == '-' || *str == '+')
// 	{
// 		if (*str == '-')
// 			sign = -1;
// 		str++;
// 	}
// 	while (*str >= '0' && *str <= '9')
// 	{
// 		num = num * 10 + (*str - '0');
// 		str++;
// 	}
// 	return (num * sign);
// }

void	ft_error()
{
	ft_putstr_fd("ERROR\n", 2);
	exit(1);
}
