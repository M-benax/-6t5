/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:03:48 by aaboudra          #+#    #+#             */
/*   Updated: 2025/05/26 18:31:42 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void export(char *arg, t_env *envp)
{
    char *var;
    char *val;
    int found;
    t_env *tmp;

    tmp = envp;
    found = 0;
    var = get_var(arg);
    val = get_val(arg);
    while (tmp)
    {
        if (ft_strcmp(tmp->var, var) == 0)
        {
            free(tmp->val);
            tmp->val = val;
            found = 1;
            break;
        }
        if (!tmp->next)
            break;
        tmp = tmp->next;
    }
    if (!found)
        tmp->next = new_env_node(var, val);
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
    while (export[i] && !(export[i] == '='))
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

void handel_export(t_data *command)
{
    int i;

    i = 1;
    while (command->com->args[i])
    {
        if (!validation_key(command->com->args[i]))
        {
            printf("minishell: export: `%s': not a valid identifier\n", command->com->args[i]);
            break;
        }
        export(command->com->args[i], command->tenv);
        i++;
    }
}
