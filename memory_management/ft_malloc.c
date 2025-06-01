/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:11:20 by aaboudra          #+#    #+#             */
/*   Updated: 2025/05/26 18:36:29 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_gc_node *g_gc_list = NULL;

char	*ft_strdup(const char *s1)
{
	char	*p;
	size_t	i;
    
	i = ft_strlen(s1);
	p = (char *)gc_malloc((i + 1) * sizeof(char));
	ft_memcpy(p, s1, i + 1);
	return (p);
}

t_comand *new_node(char *token)
{
    t_comand *node;

    node = (t_comand *)gc_malloc(sizeof(t_comand));
    node->word = ft_strdup(token);
    put_type(node);
    node->next = NULL;
    return (node);
}

char	*ft_strndup(const char *s1, size_t size)
{
	char	*p;

	p = (char *)gc_malloc((size + 1) * sizeof(char));
	ft_memcpy(p, s1, size);
    p[size] = '\0';
	return (p);
}

void gc_free_ptr(void *ptr)
{
    t_gc_node *curr = g_gc_list;
    t_gc_node *prev = NULL;

    while (curr)
    {
        if (curr->ptr == ptr)
        {
            if (prev)
                prev->next = curr->next;
            else
                g_gc_list = curr->next;
            free(curr->ptr);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void *gc_malloc(size_t size)
{
    void *ptr;
    t_gc_node *node;

    ptr = malloc(size);
    if (!ptr)
    {
        gc_free_all();
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    node = malloc(sizeof(t_gc_node));
    if (!node)
    {
        gc_free_all();
        fprintf(stderr, "malloc failed\n");
        exit(EXIT_FAILURE);
    }

    node->ptr = ptr;
    node->next = g_gc_list;
    g_gc_list = node;

    return ptr;
}

void gc_free_all(void)
{
    t_gc_node *tmp;

    while (g_gc_list)
    {
        tmp = g_gc_list;
        g_gc_list = g_gc_list->next;
        free(tmp->ptr);
        free(tmp);
    }
}
