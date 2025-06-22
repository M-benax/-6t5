/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 18:48:31 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/13 18:49:21 by aaboudra         ###   ########.fr       */
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

static void add_minimal_env(t_env **head, t_env **current, t_data *data)
{
	char *str = getcwd(NULL, 0);
	t_env *node;

	node = new_env_node("PWD", str, data);
	free(str);
	if (!*head)
		*head = node;
	else
		(*current)->next = node;
	*current = node;

	node = new_env_node("SHLVL", "1", data);
	(*current)->next = node;
	*current = node;

	node = new_env_node("OLDPWD", NULL, data);
	(*current)->next = node;
	*current = node;

	node = new_env_node("PATH", "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", data);
	(*current)->next = node;
	*current = node;
}

t_env *init_env(char **envp, t_data *data)
{
	t_env *head = NULL;
	t_env *current = NULL;
	t_env *node;
	char *var, *val;
	int i = 0;

	if (!envp || !envp[0])
	{
		add_minimal_env(&head, &current, data);
		return (head);
	}

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