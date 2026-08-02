/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disorder_metric.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 18:59:15 by dswietoc          #+#    #+#             */
/*   Updated: 2026/08/02 10:29:33 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_compute_disorder(t_stack *a)
{
	t_stack	*first;
	t_stack	*second;
	int		mistakes;
	int		total_pairs;

	mistakes = 0;
	total_pairs = 0;
	first = a;
	while (first != NULL)
	{
		second = first->next;
		while (second != NULL)
		{
			total_pairs++;
			if (first->value > second->value)
				mistakes++;
			second = second->next;
		}
		first = first->next;
	}
	if (total_pairs == 0)
		return (0.0);
	return ((double)mistakes / total_pairs);
}
