/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bench.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 13:10:27 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/05 12:33:39 by dswietoc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	ft_first_bench_line(t_operations *op)
{
	ft_putstr_fd("[bench]", 2);
	ft_putstr_fd(" sa: ", 2);
	ft_putnbr_fd(op->sa, 2);
	ft_putstr_fd(" sb: ", 2);
	ft_putnbr_fd(op->sb, 2);
	ft_putstr_fd(" ss: ", 2);
	ft_putnbr_fd(op->ss, 2);
	ft_putstr_fd(" pa: ", 2);
	ft_putnbr_fd(op->pa, 2);
	ft_putstr_fd(" pb: ", 2);
	ft_putnbr_fd(op->pb, 2);
	ft_putstr_fd("\n", 2);
}

static void	ft_second_bench_line(t_operations *op)
{
	ft_putstr_fd("[bench]", 2);
	ft_putstr_fd(" ra: ", 2);
	ft_putnbr_fd(op->ra, 2);
	ft_putstr_fd(" rb: ", 2);
	ft_putnbr_fd(op->rb, 2);
	ft_putstr_fd(" rr: ", 2);
	ft_putnbr_fd(op->rr, 2);
	ft_putstr_fd(" rra: ", 2);
	ft_putnbr_fd(op->rra, 2);
	ft_putstr_fd(" rrb: ", 2);
	ft_putnbr_fd(op->rrb, 2);
	ft_putstr_fd(" rrr: ", 2);
	ft_putnbr_fd(op->rrr, 2);
	ft_putstr_fd("\n", 2);
}

static void	ft_print_complexity(t_operations *op)
{
	if (op->strategy == STRATEGY_SIMPLE)
		ft_putstr_fd("O(n^2)", 2);
	else if (op->strategy == STRATEGY_MEDIUM)
		ft_putstr_fd("O(n sqrt(n))", 2);
	else if (op->strategy == STRATEGY_COMPLEX)
		ft_putstr_fd("O(n log(n))", 2);
}

static void	ft_print_strategy(t_operations *op)
{
	ft_putstr_fd("[bench] strategy: ", 2);
	if (op->is_adaptive)
		ft_putstr_fd("Adaptive / ", 2);
	else if (op->strategy == STRATEGY_SIMPLE)
		ft_putstr_fd("Simple / ", 2);
	else if (op->strategy == STRATEGY_MEDIUM)
		ft_putstr_fd("Medium / ", 2);
	else if (op->strategy == STRATEGY_COMPLEX)
		ft_putstr_fd("Complex / ", 2);
	ft_print_complexity(op);
	ft_putchar_fd('\n', 2);
}

void	ft_bench(t_operations *op)
{
	int	scaled;
	int	decimal;

	scaled = (int)(op->disorder * 10000 + 0.5);
	decimal = scaled % 100;
	ft_putstr_fd("[bench] disorder: ", 2);
	ft_putnbr_fd(scaled / 100, 2);
	ft_putchar_fd('.', 2);
	if (decimal < 10)
		ft_putchar_fd('0', 2);
	ft_putnbr_fd(decimal, 2);
	ft_putstr_fd("%\n", 2);
	ft_print_strategy(op);
	ft_putstr_fd("[bench] total_ops: ", 2);
	ft_putnbr_fd(op->total_ops, 2);
	ft_putchar_fd('\n', 2);
	ft_first_bench_line(op);
	ft_second_bench_line(op);
}

// static void	init_operations(t_operations *op)
// {
// 	op->sa = 15;
// 	op->sb = 30;
// 	op->ss = 45;
// 	op->pa = 60;
// 	op->pb = 75;
// 	op->ra = 90;
// 	op->rb = 102;
// 	op->rr = 101;
// 	op->rra = 22;
// 	op->rrb = 33;
// 	op->rrr = 44;
// 	op->total_ops = 76573;
// 	op->disorder = 0.512727;
// 	op->strategy = STRATEGY_ADAPTIVE;
// 	op->is_adaptive = true;
// 	op->is_bench_on = BENCH_MODE;
// }
//
// int main(void)
// {
// 	t_operations	op;
//
// 	init_operations(&op);
// 	ft_bench(&op);
// 	return (0);
// }