/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 20:03:48 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/17 19:26:41 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int kontine_puse(char *argg)
{
	int i = 0;
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
	char *var = get_var_plus_equal(arg, data);
	char *val = get_val_plus_equal(arg, data);
	int found = 0;
	t_env *tmp = envp;
	int is_plus_equal = kontine_puse(arg);
	char *joined;

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
	gc_free_ptr(var, data);
}

static int is_number(char c)
{
	return(c >= '0' && c <= '9');
}

static int validation_key(char *export)
{
	int i = 0;
	
	if (is_number(export[0]))
		return (0);
	while (export[i] && export[i] != '=' && !(export[i] == '+' && export[i + 1] == '='))
	{
		if (export[i] == ' ')
			return (0);
		if (!(export[i] == '_') && !(export[i] >= 'a' && export[i] <= 'z') 
			&& !(export[i] >= 'A' && export[i] <= 'Z') && !is_number(export[i]))
			return (0);
		i++;
	}
	return (1);
}

int handel_export(t_data *data)
{
	int i = 1;
	int status = 0;

	while (data->com->args[i])
	{
		if (!validation_key(data->com->args[i]))
		{
			printf("minishell: export: `%s': not a valid identifier\n", data->com->args[i]);
			status = EXIT_GENERAL_ERROR;
			i++;
			continue;
		}
		export(data->com->args[i], data->tenv, data);
		i++;
	}
	return (status);
}