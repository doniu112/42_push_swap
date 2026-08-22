
#include "push_swap.h"

void	ft_swap(t_stack **stack)
{
	t_stack	*first;
	t_stack	*second;

	if (!stack || !*stack || !(*stack)->next)
		return ;
	first = *stack;
	second = first->next;
	first->next = second->next;
	second->next = first;
	*stack = second;
}

void	sa(t_stack **stack_a)
{
	ft_swap(stack_a);
	write(1, "sa\n", 3);
}

void	sb(t_stack **stack_b)
{
	ft_swap(stack_b);
	write(1, "sb\n", 3);
}

void	ss(t_stack **stack_a, t_stack **stack_b)
{
	ft_swap(stack_a);
	ft_swap(stack_b);
	write(1, "ss\n", 3);
}





void	ft_pb(t_stack **stack_1, t_stack **stack_2)
{
	t_stack	*tmp;

	if (!stack_1 || !*stack_1)
		return ;
	tmp = *stack_1;
	*stack_1 = (*stack_1)->next;
	tmp->next = *stack_2;
	*stack_2 = tmp;
}

void	ft_pa(t_stack **stack_a, t_stack **stack_b)
{
	ft_pa(stack_a, stack_b);
	write(1, "pa\n", 3);
}

void	ft_pb(t_stack **stack_a, t_stack **stack_b)
{
	ft_pb(stack_b, stack_a);
	write(1, "pb\n", 3);
}



void	ft_rotate(t_stack **stack)
{
	t_stack	tmp;
	int	element;

	if (!stack || !*stack)
		return ;
	element = *stack;
	while (*stack)
	{

	}
}
