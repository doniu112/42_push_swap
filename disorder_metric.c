/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disorder_metric.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 18:59:15 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/05 11:43:26 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

double	ft_compute_disorder(t_stack *a)
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

// t_stack	*init_stack(int *numbers, int size)
// {
// 	t_stack	*stack;
// 	t_stack	*new;
// 	t_stack	*last;
// 	int		i;

// 	stack = NULL;
// 	last = NULL;
// 	i = 0;
// 	while (i < size)
// 	{
// 		new = malloc(sizeof(t_stack));
// 		if (!new)
// 			return (NULL);
// 		new->value = numbers[i];
// 		new->next = NULL;
// 		if (!stack)
// 			stack = new;
// 		else
// 			last->next = new;
// 		last = new;
// 		i++;
// 	}
// 	return (stack);
// }
//
// int	main(void)
// {
// 	t_stack *stack_a;
// 	double	disorder;
// 	int size;
// 	int numbers[] = {
//     187, 151, 68, 82, 62, 22, 71, 115, 196, 118,
//     145, 156, 171, 97, 86, 7, 126, 83, 46, 124,
//     54, 90, 66, 87, 194, 152, 70, 142, 2, 132,
//     48, 21, 61, 104, 125, 173, 168, 121, 166, 114,
//     4, 23, 75, 56, 103, 192, 78, 148, 94, 163,
//     141, 135, 88, 108, 140, 84, 179, 116, 69, 154,
//     64, 59, 30, 49, 80, 138, 47, 137, 55, 123,
//     174, 72, 25, 133, 37, 29, 182, 195, 38, 1,
//     144, 198, 16, 35, 5, 6, 130, 184, 89, 191,
//     81, 96, 155, 13, 199, 73, 36, 60, 164, 157
// 	};
//
// 	size = sizeof(numbers)/sizeof(numbers[0]);
// 	stack_a = init_stack(numbers, size);
// 	disorder = ft_compute_disorder(stack_a);
//	
// 	printf("disorder : %f", disorder);
//	free(stack_a);
//	return (0);
// }
