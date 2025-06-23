/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:03:39 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/23 15:49:40 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void handel_heredoc(char *limiter, t_cmd *cmd, t_data *data)
{
	int fd;
	char *line;
	char *file_name;

	fd = open_file(&file_name, data);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	cmd->in_file = file_name;
}

static void copy_args(char **src, char **dst, int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		dst[i] = src[i];
		i++;
	}
}

static void copy_flags(int *src, int *dst, int count)
{
	int i;

	i = 0;
	while (i < count)
	{
		dst[i] = src[i];
		i++;
	}
}

static void add_arg(t_cmd *cmd, char *word, int q, t_data *data)
{
	int		i;
	char	**new_args;
	int		*new_quoted_flags;

	i = 0;
	while (cmd->args && cmd->args[i])
		i++;
	new_args = gc_malloc(sizeof(char *) * (i + 2), data);
	new_quoted_flags = gc_malloc(sizeof(int) * (i + 1), data);
	copy_args(cmd->args, new_args, i);
	copy_flags(cmd->quoted_flags, new_quoted_flags, i);
	new_args[i] = ft_strdup(word, data);
	new_args[i + 1] = NULL;
	new_quoted_flags[i] = q;
	cmd->args = new_args;
	cmd->quoted_flags = new_quoted_flags;
	cmd->argc = i + 1;
}


static void put_command_part(t_comand *token, t_cmd *cmd, t_data *data)
{
	if (token->type == T_WORD)
		add_arg(cmd, token->word, token->quoted, data);
	else if (token->type == T_REDIR_IN)
	{
		cmd->in_type = T_REDIR_IN;
			cmd->in_file = ft_strdup(token->next->word, data);
	}
	else if (token->type == T_REDIR_OUT)
	{
		cmd->out_type = T_REDIR_OUT;
			cmd->out_file = ft_strdup(token->next->word, data);
	}
	else if (token->type == T_APPEND)
	{
		cmd->out_type = T_APPEND;
			cmd->out_file = ft_strdup(token->next->word, data);
	}
	else if (token->type == T_HEREDOC)
	{
		cmd->in_type = T_HEREDOC;
		handel_heredoc(ft_strdup(token->next->word, data), cmd, data);
	}
}

static t_cmd *init_node(t_data *data)
{
	t_cmd *node;
	
	node = gc_malloc(sizeof(t_cmd), data);
	node->args = NULL;
	node->in_file = NULL;
	node->out_file = NULL;
	node->in_type = 0;
	node->out_type = 0;
	node->next = NULL;
	return node;
}

void	parse_command(t_comand *tokens, t_data *data)
{
	t_cmd	*head;
	t_cmd	*curent;
	t_cmd	*node;

	head = NULL;
    curent = NULL;
	while (tokens)
	{
		node = init_node(data);
		while (tokens && tokens->type != T_PIPE)
		{
			put_command_part(tokens, node, data);
			tokens = tokens->next;
		}
		if (tokens && tokens->type == T_PIPE)
			tokens = tokens->next;
		if (!head)
			head = node;
		else
			curent->next = node;
		curent = node;
	}
	data->com = head;
}