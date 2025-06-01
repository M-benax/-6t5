/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:48:31 by aaboudra          #+#    #+#             */
/*   Updated: 2025/05/26 17:40:29 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



t_env *new_env_node(char *var, char *val)
{
    t_env *node;

    node = gc_malloc(sizeof(t_env));
    node->var = ft_strdup(var);
    node->val = ft_strdup(val);
    node->next = NULL;
    return (node);
}
char *get_var(const char *env_line)
{
    int i;

    i = 0;
    while (env_line[i] && env_line[i] != '=')
        i++;
    return (ft_strndup(env_line, i));
}

char *get_val(const char *env_line)
{
    int i;

    i = 0;
    while (env_line[i] && env_line[i] != '=')
        i++;
    if (env_line && env_line[i] == '=')
    {
        return (ft_strdup(env_line + i + 1));
    }
    return (ft_strdup(""));
}

t_env *init_env(char **envp)
{
    t_env *head;
    t_env *current;
    t_env *node;
    char *var;
    char *val;
    int i;
    
    head = NULL;
    current = NULL;
    i = 0;
    while (envp[i])
    {
        var = get_var(envp[i]);
        val = get_val(envp[i]);
        node = new_env_node(var, val);
        gc_free_ptr(var);
        gc_free_ptr(val);
        if (!head)
            head = node;
        else
            current->next = node;
        current = node;
        i++;
    }
    return (head);
}
