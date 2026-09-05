/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bench.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dswietoc <dswietoc@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 13:10:27 by dswietoc          #+#    #+#             */
/*   Updated: 2026/09/05 11:53:26 by dswietoc         ###   ########.fr       */
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

static void	ft_strategy_print(t_operations *op)
{
	ft_putstr_fd("[bench] strategy: ", 2);
	if (op->strategy == STRATEGY_SIMPLE && op->is_adaptive == false)
		ft_putstr_fd("Simple / O(n2)", 2);
	else if (op->strategy == STRATEGY_MEDIUM && op->is_adaptive == false)
		ft_putstr_fd("Medium /  O(n√n)", 2);
	else if (op->strategy == STRATEGY_COMPLEX && op->is_adaptive == false)
		ft_putstr_fd("Complex / O(n log n)", 2);
	else if (op->strategy == STRATEGY_SIMPLE && op->is_adaptive == true)
		ft_putstr_fd("Adaptive / O(n2)", 2);
	else if (op->strategy == STRATEGY_MEDIUM && op->is_adaptive == true)
		ft_putstr_fd("Adaptive /  O(n√n)", 2);
	else if (op->strategy == STRATEGY_COMPLEX && op->is_adaptive == true)
		ft_putstr_fd("Adaptive / O(n log n)", 2);
	ft_putstr_fd("\n", 2);
}

void	ft_bench(t_operations *op)
{
	ft_putstr_fd("[bench] disorder: ", 2);
	ft_putnbr_fd(op->disorder * 10000 / 100, 2);
	ft_putstr_fd(".", 2);
	ft_putnbr_fd((int)(op->disorder * 10000) % 100, 2);
	ft_putstr_fd("%", 2);
	ft_putstr_fd("\n", 2);
	ft_strategy_print(op);
	ft_putstr_fd("[bench] total_ops: ", 2);
	ft_putnbr_fd(op->total_ops, 2);
	ft_putstr_fd("\n", 2);
	ft_first_bench_line(op);
	ft_second_bench_line(op);
}

// static void	init_operations(t_operations *op)
// {
// 	op->sa = 0;
// 	op->sb = 0;
// 	op->ss = 0;
// 	op->pa = 0;
// 	op->pb = 0;
// 	op->ra = 0;
// 	op->rb = 0;
// 	op->rr = 0;
// 	op->rra = 0;
// 	op->rrb = 0;
// 	op->rrr = 0;
// 	op->total_ops = 0;
// 	op->disorder = 0.572727;
// 	op->strategy = STRATEGY_ADAPTIVE;
// 	op->is_adaptive = true;
// 	op->is_bench_on = BENCH_MODE;
// }

// int main(void)
// {
// 	t_operations	op;

// 	init_operations(&op);
// 	ft_bench(&op);
// 	return (0);
// }