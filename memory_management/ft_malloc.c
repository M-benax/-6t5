/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 18:11:20 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/17 18:04:33 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



char	*ft_strdup(const char *s1, t_data *data)
{
	char	*p;
	size_t	i;
    
	i = ft_strlen(s1);
	p = (char *)gc_malloc((i + 1) * sizeof(char), data);
	ft_memcpy(p, s1, i + 1);
	return (p);
}

t_comand *new_node(char *token, int q, t_data *data)
{
    t_comand *node;

    node = (t_comand *)gc_malloc(sizeof(t_comand), data);
    node->word = ft_strdup(token, data);
    node->quoted = q;
    put_type(node);
    node->next = NULL;
    return (node);
}

char	*ft_strndup(const char *s1, size_t size, t_data *data)
{
	char	*p;

	p = (char *)gc_malloc((size + 1) * sizeof(char), data);
	ft_memcpy(p, s1, size);
    p[size] = '\0';
	return (p);
}

void gc_free_pid_list(t_pid_list *list, t_data *data)
{
    t_pid_list *tmp;
    while (list)
    {
        tmp = list;
        list = list->next;
        gc_free_ptr(tmp, data);
    }
}

void gc_free_array(char **array, t_data *data)
{
    int i;
    if (!array) return;
    i = 0;
    while (array[i])
    {
        gc_free_ptr(array[i], data); // Assumes strings in array were gc_malloc'd
        i++;
    }
    gc_free_ptr(array, data); // Free the array of pointers itself
}

void gc_free_ptr(void *ptr, t_data *data)
{
    t_gc_node *curr = data->g_gc_list;
    t_gc_node *prev = NULL;

    while (curr)
    {
        if (curr->ptr == ptr)
        {
            if (prev)
                prev->next = curr->next;
            else
                data->g_gc_list = curr->next;
            free(curr->ptr);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void *gc_malloc(size_t size, t_data *data)
{
    void *ptr;
    t_gc_node *node;

    ptr = malloc(size);
    if (!ptr)
    {
        gc_free_all(data);
        printf("malloc failed\n");
        exit(EXIT_FAILURE);
    }
    node = malloc(sizeof(t_gc_node));
    if (!node)
    {
        gc_free_all(data);
        printf("malloc failed\n");
        exit(EXIT_FAILURE);
    }
    node->ptr = ptr;
    node->next = data->g_gc_list;
    data->g_gc_list = node;
    return (ptr);
}

void gc_free_all(t_data *data)
{
    t_gc_node *tmp;

    while (data->g_gc_list)
    {
        tmp = data->g_gc_list;
        data->g_gc_list = data->g_gc_list->next;
        free(tmp->ptr);
        free(tmp);
    }
}
