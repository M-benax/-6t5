/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilds.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:46:55 by aaboudra          #+#    #+#             */
/*   Updated: 2025/05/31 17:17:44 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char **extract_vars(t_env *env)
{
    int i;
    char **arr;
    
    i = 0;
    int size = count_nodes(env);
    arr = gc_malloc(sizeof(char *) * (size + 1));
    while (env)
    {
        arr[i++] = env->var;
        env = env->next;
    }
    arr[i] = NULL;
    return (arr);
}

static void sort_vars(char **vars)
{
    int i;
    int j;
    
    i = 0;
    j = 0;
    char *tmp;
    while (vars[i])
    {
        j = i + 1;
        while (vars[j])
        {
            if (ft_strcmp(vars[i], vars[j]) > 0)
            {
                tmp = vars[i];
                vars[i] = vars[j];
                vars[j] = tmp;
            }
            j++;
        }
        i++;
    }
}

static t_env *find_var(t_env *env, char *var)
{
    while (env)
    {
        if (ft_strcmp(env->var, var) == 0)
            return env;
        env = env->next;
    }
    return NULL;
}

void print_export(t_env *env)
{
    int i;
    char **vars;
    t_env *node;
    
    vars = extract_vars(env);
    sort_vars(vars);
    i = 0;
    while (vars[i])
    {
        node = find_var(env, vars[i]);
        if (node->val)
            printf("declare -x %s=\"%s\"\n", node->var, node->val);
        else
            printf("declare -x %s\n", node->var);
        i++;
    }
    gc_free_ptr(vars);
}

void biltings(t_data *comand)
{
    
    if (comand->com->args && comand->com->args[0] &&
        (ft_strcmp(comand->com->args[0], "env") == 0) &&
            !comand->com->args[1] && !comand->com->next)
            print_env(comand->tenv);
    if	(comand->com->args && comand->com->args[0] &&
         (ft_strcmp(comand->com->args[0], "export") == 0))
    {
        if(!comand->com->args[1] && !comand->com->next)
            print_export(comand->tenv);
        else
            handel_export(comand);
    }
    if	(comand->com->args && comand->com->args[0] &&
        (ft_strcmp(comand->com->args[0], "unset") == 0))
        unset(comand->com->args, &comand->tenv);
    
}
