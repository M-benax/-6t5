/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:52:23 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/23 15:47:18 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *expand_dollar_pid(char *result, t_data *data)
{
    char *pid_str = ft_itoa(getpid(), data);
    size_t len = ft_strlen(result) + ft_strlen(pid_str) + 1;
    char *tmp = gc_malloc(len, data);

    ft_strcpy(tmp, result);
    ft_strcpy(tmp + ft_strlen(result), pid_str);
    gc_free_ptr(result, data);
    gc_free_ptr(pid_str, data);

    return tmp;
}

// char *expand_dollar_status(char *result, t_data *data)
// {
//     char *status_str;
//     size_t len;
//     char *tmp;
    
//     len = ft_strlen(result) + ft_strlen(status_str) + 1;
//     status_str = ft_itoa(data->last_exit_status, data);
//     tmp = gc_malloc(len, data);
//     ft_strcpy(tmp, result);
//     ft_strcpy(tmp + ft_strlen(result), status_str);
//     gc_free_ptr(result, data);
//     gc_free_ptr(status_str, data);
//     return (tmp);
// }

char *expand_env_variable(char *result, const char *str, int *i, t_data *data)
{
    int j = *i + 1;
    while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
        j++;

    char *var_name = ft_strndup(str + *i + 1, j - *i - 1, data);
    char *value = get_env_value(data->tenv, var_name, data);
    if (!value)
        value = "";

    size_t result_len = ft_strlen(result);
    size_t value_len = ft_strlen(value);
    char *tmp = gc_malloc(result_len + value_len + 1, data);
    ft_strcpy(tmp, result);
    ft_strcpy(tmp + result_len, value);

    gc_free_ptr(result, data);
    gc_free_ptr(var_name, data);
    *i = j;

    return tmp;
}


char *append_char_to_result(char *result, char c, t_data *data)
{
    size_t len = ft_strlen(result);
    char *tmp = gc_malloc(len + 2, data);

    ft_strcpy(tmp, result);
    tmp[len] = c;
    tmp[len + 1] = '\0';

    gc_free_ptr(result, data);
    return tmp;
}

static int	handle_dollar(const char *s, char **res, int *i, t_data *d)
{
	if (s[*i + 1] == '$')
	{
		*res = expand_dollar_pid(*res, d);
		*i += 2;
	}
	else if (s[*i + 1] == '?')
	{
		*res = ft_itoa(d->last_exit_status, d);
		*i += 2;
	}
	else if (ft_isalpha(s[*i + 1]) || s[*i + 1] == '_')
	{
		*res = expand_env_variable(*res, s, i, d);
	}
	else
	{
		*res = append_char_to_result(*res, '$', d);
		(*i)++;
	}
	return (0);
}

char	*expand_var(const char *str, t_data *data)
{
	int		i;
	char	*result;

	i = 0;
	result = gc_malloc(1, data);
	if (!result)
		return (NULL);
	result[0] = '\0';
	while (str[i])
	{
		if (str[i] == '$')
			handle_dollar(str, &result, &i, data);
		else
		{
			result = append_char_to_result(result, str[i], data);
			i++;
		}
	}
	return (result);
}

static int count_args(char **args)
{
    int count = 0;
    
    while (args && args[count])
        count++;
    return count;
}

static void replace_arg_at(t_cmd *cmd, int index, char **split, t_data *data)
{
    int split_count;
    int new_argc;
    char **new_args;
    int *new_flags;
    int j;
    int k;
	
	split_count = count_args(split);
    new_argc = cmd->argc - 1 + split_count;
    new_args = gc_malloc(sizeof(char *) * (new_argc + 1), data);
    new_flags = gc_malloc(sizeof(int) * new_argc, data);
    j = 0;// copy before index
    while (j < index)
    {
        new_args[j] = cmd->args[j];
        new_flags[j] = cmd->quoted_flags[j];
        j++;
    }
    k = 0;// insert split strings
    while (k < split_count)
    {
        new_args[j] = ft_strdup(split[k], data);
        new_flags[j] = 0;  // 0 because it's split without quotes
        k++;
        j++;
    }
    k = index + 1;// copy after index
    while (k < cmd->argc)
    {
        new_args[j] = cmd->args[k];
        new_flags[j] = cmd->quoted_flags[k];
        k++;
        j++;
    }
    new_args[j] = NULL;
    gc_free_ptr(cmd->args[index], data); // Free the old element
    gc_free_ptr(cmd->args, data);
    gc_free_ptr(cmd->quoted_flags, data);
    cmd->args = new_args;
    cmd->quoted_flags = new_flags;
    cmd->argc = new_argc;
}

static void replace_arg(t_cmd *cmd, int *i_ptr, char **split, t_data *data)
{
    replace_arg_at(cmd, *i_ptr, split, data);
    *i_ptr += count_args(split) - 1;
}

// Expand arguments of a single command
static char *expand_args_one_cmd_1(t_cmd *cmd, char *expanded, t_data *data, int *i)
{
    char **split;
    int k;
    
    split = NULL;
    split = ft_split(expanded, ' ', data);
    gc_free_ptr(expanded, data); // Free expanded before replacing
    replace_arg(cmd, i, split, data);
    k = 0;// Free the split array
    while (split[k] != NULL) 
    {
        gc_free_ptr(split[k], data);
        k++;
    }
    gc_free_ptr(split, data);
    return (expanded);
}
//  Expand arguments of a single command
static void expand_args_one_cmd(t_data *data)
{
    int i = 0;
    char *expanded;

    while (data->com->args[i])
    {
        // skip single quotes
        if (data->com->quoted_flags[i] == 1)
        {
            i++;
            continue;
        }

        // handle variables
        if (ft_strchr(data->com->args[i], '$'))
        {
            char *eq = ft_strchr(data->com->args[i], '=');
            char *dlr = ft_strchr(data->com->args[i], '$');

            // case: expansion occurs after '=' → no split
            if (eq && dlr > eq)
            {
                expanded = expand_var(data->com->args[i], data);
                gc_free_ptr(data->com->args[i], data);
                data->com->args[i] = expanded;
                i++;
                continue;
            }

            // case: quoted double → expand only, no split
            if (data->com->quoted_flags[i] == 2)
            {
                expanded = expand_var(data->com->args[i], data);
                gc_free_ptr(data->com->args[i], data);
                data->com->args[i] = expanded;
                i++;
                continue;
            }

            // case: unquoted → expand and split
            expanded = expand_var(data->com->args[i], data);
            expanded = expand_args_one_cmd_1(data->com, expanded, data, &i);
            i++;
            continue;
        }

        // no $ → keep as is
        i++;
    }
}

// 🔁 Expand variables in all commands
int expand_all_commands(t_data *data)
{
    t_cmd *cmd;

    cmd = data->com;
    while (cmd)
    {
        expand_args_one_cmd(data);
        cmd = cmd->next;
    }
    return 0;
}

