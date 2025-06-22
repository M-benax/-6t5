/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 18:52:23 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/17 15:57:50 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *expand_dollar_pid(char *result, t_data *data)
{
	char *pid_str = ft_itoa(getpid(), data);
	size_t len = ft_strlen(result) + ft_strlen(pid_str) + 1;
	char *tmp = gc_malloc(len, data);

	ft_strcpy(tmp, result);
	ft_strcat(tmp, pid_str);
	gc_free_ptr(result, data);
	gc_free_ptr(pid_str, data);
	return tmp;
}

char *expand_dollar_status(char *result, t_data *data)
{
	char *status_str = ft_itoa(data->last_exit_status, data);
	size_t len = ft_strlen(result) + ft_strlen(status_str) + 1;
	char *tmp = gc_malloc(len, data);

	ft_strcpy(tmp, result);
	ft_strcat(tmp, status_str);
	gc_free_ptr(result, data);
	gc_free_ptr(status_str, data);
	return tmp;
}

char *expand_env_variable(char *result, const char *str, int *i, t_data *data)
{
	int j = *i + 1;
	while (str[j] && (ft_isalnum(str[j]) || str[j] == '_'))
		j++;

	char *var_name = ft_strndup(str + *i + 1, j - *i - 1, data);
	char *value = get_env_value(data->tenv, var_name, data);
	
	size_t result_len = ft_strlen(result);
	size_t value_len = ft_strlen(value);
	char *tmp = gc_malloc(result_len + value_len + 1, data);
	
	ft_strcpy(tmp, result);
	ft_strcat(tmp, value);

	gc_free_ptr(result, data);
	gc_free_ptr(var_name, data);
	gc_free_ptr(value, data);
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

char *expand_var(const char *str, t_data *data)
{
	int i = 0;
	char *result = gc_malloc(1, data);
	result[0] = '\0';

	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '$')
			{
				result = expand_dollar_pid(result, data);
				i += 2;
			}
			else if (str[i + 1] == '?')
			{
				result = expand_dollar_status(result, data);
				i += 2;
			}
			else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
			{
				result = expand_env_variable(result, str, &i, data);
			}
			else
			{
				result = append_char_to_result(result, '$', data);
				i++;
			}
		}
		else
		{
			result = append_char_to_result(result, str[i], data);
			i++;
		}
	}
	return result;
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
	int split_count = count_args(split);
	int new_argc = cmd->argc - 1 + split_count;
	char **new_args = gc_malloc(sizeof(char *) * (new_argc + 1), data);
	int *new_flags = gc_malloc(sizeof(int) * new_argc, data);
	int j = 0, k = 0;

	while (j < index)
	{
		new_args[j] = cmd->args[j];
		new_flags[j] = cmd->quoted_flags[j];
		j++;
	}
	k = 0;
	while (k < split_count)
	{
		new_args[j] = ft_strdup(split[k], data);
		new_flags[j] = 0;
		k++;
		j++;
	}
	k = index + 1;
	while (k < cmd->argc)
	{
		new_args[j] = cmd->args[k];
		new_flags[j] = cmd->quoted_flags[k];
		k++;
		j++;
	}
	new_args[j] = NULL;
	gc_free_ptr(cmd->args[index], data);
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

static void expand_args_one_cmd_1(t_cmd *cmd, char *expanded, t_data *data, int *i)
{
	char **split = ft_split(expanded, ' ', data);
	gc_free_ptr(expanded, data);
	replace_arg(cmd, i, split, data);
	int k = 0;
	while (split && split[k])
	{
		gc_free_ptr(split[k], data);
		k++;
	}
	gc_free_ptr(split, data);
}

static void expand_args_one_cmd(t_cmd *cmd, t_data *data)
{
	int i = 0;
	while (cmd->args && cmd->args[i])
	{
		char *expanded = NULL;
		if (cmd->quoted_flags[i] == 1)
		{
			i++;
			continue;
		}
		if (ft_strchr(cmd->args[i], '$'))
			expanded = expand_var(cmd->args[i], data);
		else
			expanded = ft_strdup(cmd->args[i], data);
		
		if (cmd->quoted_flags[i] == 0)
			expand_args_one_cmd_1(cmd, expanded, data, &i);
		else
		{
			gc_free_ptr(cmd->args[i], data);
			cmd->args[i] = expanded;
		}
		i++;
	}
}

int expand_all_commands(t_data *data)
{
	t_cmd *cmd = data->com;
	while (cmd)
	{
		expand_args_one_cmd(cmd, data);
		cmd = cmd->next;
	}
	return 0;
}