#include "push_swap.h"


void        ft_pustr_fd(char *s, int fd)
{
        while (*s)
        {
                write(fd, s, 1);
                s++;
        }
}


void ft_free_stack(t_stack **stack)
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