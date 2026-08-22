/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/02 10:55:56 by dswietoc          #+#    #+#             */
/*   Updated: 2026/08/22 15:48:41 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_check_strategy_selector(char *str)
{
	if (strcmp(str, "--simple") == 0)
	{
		printf("Running simple algorithm\n");
		return (1);
	}
	else if (strcmp(str, "--medium") == 0)
	{
		printf("Running medium algorithm\n");
		return (2);
	}
	else if (strcmp(str, "--complex") == 0)
	{
		printf("Running complex algorithm\n");
		return (3);
	}
	else if (strcmp(str, "--adaptive") == 0)
	{
		printf("Running adaptive algorithm\n");
		return (4);
	}
}

int	ft_push_swap(int *argc, char **argv)
{
	
}
