/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_simple_algorithm.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 18:48:26 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/05 11:55:15 by dswietoc         ###   ########.fr       */
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

int	ft_find_min_position(t_stack *stack)
{
	int	min;
	int	min_pos;
	int	pos;

	min = stack->value;
	min_pos = 0;
	pos = 0;
	while (stack)
	{
		if (stack->value < min)
		{
			min = stack->value;
			min_pos = pos;
		}
		stack = stack->next;
		pos++;
	}
	return (min_pos);
}

void	ft_move_to_top(t_stack **a, t_operations *op)
{
	int	pos;
	int	size;

	if (!a || !*a)
		return ;
	pos = ft_find_min_position(*a);
	size = ft_stack_size(*a);
	if (pos <= size / 2)
	{
		while (pos > 0)
		{
			ft_ra(a, op);
			pos--;
		}
	}
	else
	{
		while (pos < size)
		{
			ft_rra(a, op);
			pos++;
		}
	}
}

void	ft_simple_sort(t_stack **a, t_stack **b, t_operations *op)
{
	while (*a)
	{
		ft_move_to_top(a, op);
		ft_pb(a, b, op);
	}
	while (*b)
	{
		ft_pa(a, b, op);
	}
}

// int	main(void)
// {
// 	int				size;
// 	int				i;
// 	t_stack			*a;
// 	t_stack			*b;
// 	t_stack			*new;
// 	t_stack			*last;
// 	t_stack			*tmp;
// 	t_operations	op = {0};
// 	int	values[] = {
// 	327, 14, 486, 92, 251, 403, 67, 118, 359, 221,
// 	5, 474, 188, 310, 76, 444, 133, 299, 21, 395,
// 	156, 268, 49, 417, 102, 341, 230, 9, 465, 175,
// 	287, 58, 376, 124, 498, 212, 33, 451, 196, 320,
// 	84, 365, 143, 279, 17, 432, 109, 244, 390, 61
// };

// 	a = NULL;
// 	b = NULL;
// 	last = NULL;
// 	size = sizeof(values) / sizeof(values[0]);
// 	i = 0;
// 	while (i < size)
// 	{
// 		new = malloc(sizeof(t_stack));
// 		if (!new)
// 			return (1);
// 		new->value = values[i];
// 		new->next = NULL;
// 		if (!a)
// 			a = new;
// 		else
// 			last->next = new;
// 		last = new;
// 		i++;
// 	}

// 	printf("BEFORE:\n");
// 	tmp = a;
// 	while (tmp)
// 	{
// 		printf("%d ", tmp->value);
// 		tmp = tmp->next;
// 	}
// 	printf("\n");

// 	ft_simple_sort(&a, &b, &op);

// 	printf("AFTER:\n");
// 	tmp = a;
// 	while (tmp)
// 	{
// 		printf("%d ", tmp->value);
// 		tmp = tmp->next;
// 	}
// 	printf("\n");

// 	while (a)
// 	{
// 		tmp = a->next;
// 		free(a);
// 		a = tmp;
// 	}
// 	return (0);
// }