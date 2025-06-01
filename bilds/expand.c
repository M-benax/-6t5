/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:52:23 by aaboudra          #+#    #+#             */
/*   Updated: 2025/05/24 22:46:32 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char *expand_simple_variable(const char *str, int *error, t_env *env)
{
    int i;
    char *var;
    char *val;
    char *suffix;
    char *res;
    
    i = 1;
    while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
        i++;
    if (i == 1) 
    {
        *error = 1;
        return (NULL);
    }
    var = ft_strndup(str + 1, i - 1);
    val = get_env_value(env, var);
    free(var);
    suffix = ft_strdup(str + i);
    res = gc_malloc(strlen(val) + ft_strlen(suffix) + 1);
    ft_strcpy(res, val);
    ft_strcat(res, suffix);
    free(val);
    free(suffix);
    return (res);
}

static char *expand_braced_variable(const char *str, int *error, t_env *env)
{
    const char *start;
    const char *end;
    char *var;
    char *val;
    size_t len;

    start = str + 2;
    end = ft_strchr(start, '}');
    if (!end)
    {
        *error = 1;
        return NULL;
    }
    len = end - start;
    if (len == 0) {
        *error = 1;
        return NULL;
    }
    var = ft_strndup(start, len);
    val = get_env_value(env, var);
    free(var);
    return (val);
}

static char *expand_variable(const char *str, int *error, t_env *env)
{
    if (!str || str[0] != '$') 
        return( ft_strdup(str));
    if (str[1] == '{')
        return (expand_braced_variable(str, error, env));
    else
        return (expand_simple_variable(str, error, env));
}

static  int expand_args_one_cmd(t_cmd *cmd, t_env *env)
{
    int error;
    int i;
    char *expanded ;
    
    i = 0;
    error = 0;
    while(cmd->args[i])
    {
        if (ft_strchr(cmd->args[i], '$')) {
            expanded = expand_variable(cmd->args[i], &error, env);
            if (error) 
            {
                printf("bash: syntax error in variable expansion: %s\n", cmd->args[i]);
                return (1);
            }
            free(cmd->args[i]);
            cmd->args[i] = expanded;
        }
        i++;
    }
    return (0);
}

int expand_all_commands(t_data *data)
{
    t_cmd *cmd = data->com;
    while (cmd) {
        int err = expand_args_one_cmd(cmd, data->tenv);
        if (err) return err;
        cmd = cmd->next;
    }
    return 0;
}
