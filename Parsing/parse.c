/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:03:39 by aaboudra          #+#    #+#             */
/*   Updated: 2025/05/26 18:36:58 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void handel_heredoc(char *limiter, t_cmd *cmd)
{
	int fd;
	char *line;
	char *file_name;

	fd = open_file(&file_name);
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

static void	add_arg(t_cmd *cmd, char *word)
{
	int		i;
	char	**new_args;

	i = 0;
	while (cmd->args && cmd->args[i])
		i++;
	new_args = gc_malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[i++] = ft_strdup(word);
	new_args[i] = NULL;
	if (cmd->args)
		cmd->args = NULL;
	cmd->args = new_args;
}

static void put_command_part(t_comand *token, t_cmd *cmd)
{
	
	if (token->type == T_WORD)
		add_arg(cmd, token->word);
	else if (token->type == T_REDIR_IN)
	{
		cmd->in_type = T_REDIR_IN;
			cmd->in_file = ft_strdup(token->next->word);
	}
	else if (token->type == T_REDIR_OUT)
	{
		cmd->out_type = T_REDIR_OUT;
			cmd->out_file = ft_strdup(token->next->word);
	}
	else if (token->type ==T_APPEND)
	{
		cmd->out_type = T_APPEND;
			cmd->out_file = ft_strdup(token->next->word);
	}
	else if (token->type == T_HEREDOC)
	{
		cmd->in_type = T_HEREDOC;
		handel_heredoc(ft_strdup(token->next->word), cmd);
	}
}

static t_cmd *init_node()
{
	t_cmd *node;
	
	node = gc_malloc(sizeof(t_cmd));
	node->args = NULL;
	node->in_file = NULL;
	node->out_file = NULL;
	node->in_type = 0;
	node->out_type = 0;
	node->next = NULL;
	return node;
}

t_cmd	*parse_command(t_comand *tokens)
{
	t_cmd	*head;
	t_cmd	*curent;
	t_cmd	*node;

	head = NULL;
    curent = NULL;
	while (tokens)
	{
		node = init_node();
		while (tokens && tokens->type != T_PIPE)
		{
			put_command_part(tokens, node);
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
	return (head);
}
