/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:48:31 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/23 15:49:29 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env *new_env_node(char *var, char *val, t_data *data)
{
    t_env *node;

    node = gc_malloc(sizeof(t_env), data);
    node->var = ft_strdup(var, data);
    if (!val)
        node->val = NULL;
    else
        node->val = ft_strdup(val, data);
    node->next = NULL;
    return (node);
}
char *get_var_plus_equal(char *arg, t_data *data)
{
    int i = 0;
    char *res;

    while (arg[i] && arg[i] != '=' && !(arg[i] == '+' && arg[i + 1] == '='))
        i++;

    res = gc_malloc(i + 1, data);
    if (!res)
        return NULL;

    int j = 0;
    while (j < i)
    {
        res[j] = arg[j];
        j++;
    }
    res[j] = '\0';

    return res;
}

char *get_val_plus_equal(char *arg, t_data *data)
{
    int i = 0;

    while (arg[i])
    {
        if (arg[i] == '+' && arg[i + 1] == '=')
            return (ft_strdup(arg + i + 2, data));
        if (arg[i] == '=')
            return (ft_strdup(arg + i + 1, data));
        i++;
    }
    return (NULL);
}

t_env *init_env(char **envp, t_data *data)
{
    t_env *head;
    t_env *current;
    t_env *node;
    char *var;
    char *val;
    char *str;
    int i;
    
    head = NULL;
    current = NULL;
    if (!envp[0])
    {
        str = getcwd(NULL, 0);
        node = new_env_node("pwd", str, data);
        free(str);
        return (node);
    }
    i = 0;
    while (envp[i])
    {
        var = get_var_plus_equal(envp[i], data);
        val = get_val_plus_equal(envp[i], data);
        node = new_env_node(var, val, data);
        gc_free_ptr(var, data);
        gc_free_ptr(val, data);
        if (!head)
            head = node;
        else
            current->next = node;
        current = node;
        i++;
    }
    return (head);
}