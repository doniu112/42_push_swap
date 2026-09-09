/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorting_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/07 18:34:33 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/07 18:34:33 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_stack_size(t_stack *stack)
{
	int	size;

	size = 0;
	while (stack)
	{
		size++;
		stack = stack->next;
	}
	return (size);
}

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

void	ft_print_error(t_stack **stack_a, t_stack **stack_b)
{
	write(2, "Error\n", 6);
	ft_free_stack(stack_a);
	ft_free_stack(stack_b);
	exit(1);
}
