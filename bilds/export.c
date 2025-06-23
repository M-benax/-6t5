/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:03:48 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/23 15:47:31 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int kontine_puse(char *argg)
{
    int i;

    i = 0;
    while (argg[i])
    {
        if(argg[i] == '+' && argg[i + 1] == '=')
            return 1;
        i++;
    }
    return 0;
}

static void export(char *arg, t_env *envp, t_data *data)
{
    char *var;
    char *val;
    int found;
    t_env *tmp;
    int is_plus_equal;
    char *joined;

    tmp = envp;
    found = 0;
    is_plus_equal = kontine_puse(arg);
    var = get_var_plus_equal(arg, data);
    val = get_val_plus_equal(arg, data);
    while (tmp)
    {
        if (ft_strcmp(tmp->var, var) == 0)
        {
            found = 1;
            if (val != NULL)
            {
                if (is_plus_equal && tmp->val != NULL)
                {
                    joined = ft_strjoin(tmp->val, val, data);
                    gc_free_ptr(tmp->val,data);
                    gc_free_ptr(val, data);
                    tmp->val = joined;
                }
                else
                {
                    gc_free_ptr(tmp->val, data);
                    tmp->val = val;
                }
            }
            else
                gc_free_ptr(val, data);
            break;
        }
        if (!tmp->next)
            break;
        tmp = tmp->next;
    }
    if (!found)
        tmp->next = new_env_node(var, val, data);
}

static int is_number(char c)
{
    return(c >= '0' && c <= '9');    
}

static int validation_key(char *export)
{
    int i;

    if (is_number(export[0]))
        return (0);
    i = 0;
    while (export[i] && export[i] != '=' && !(export[i] == '+' && export[i + 1] == '='))
    {
        if (export[i] == ' ')
            return (0);
        if (!(export[i] == '_') && !(export[i] >= 'a' && export[i] <= 'z') 
            && !(export[i] >= 'A' && export[i] <= 'Z'))
            return (0);
        i++;
    }
    return (1);
}



int  handel_export(t_data *data)
{
    int i;
    int status;
    
    status = 0;
    i = 1;
    while (data->com->args[i])
    {
        if (!validation_key(data->com->args[i]))
        {
            printf("`%s': not a valid identifier\n", data->com->args[i]);
            status = EXIT_GENERAL_ERROR;
            i++;
            continue;
        }
        printf("[%s]--[%d]\n", data->com->args[i], data->com->quoted_flags[i]);
        export(data->com->args[i], data->tenv, data);
        i++;
    }
    return (status);
}