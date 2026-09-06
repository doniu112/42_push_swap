/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 11:08:59 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/06 12:05:39 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

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

void	ft_error(void)
{
    t_stack *current;
    t_stack *next;

    if (stack == NULL || *stack == NULL)
        return;

    current = *stack;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    *stack = NULL;
}
