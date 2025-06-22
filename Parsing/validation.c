/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 23:25:36 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/16 20:01:39 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int error_pipe(char *line)
{
	int i = 0;
	char quote = '\0';

	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			if (quote == '\0')
				quote = line[i];
			else if (quote == line[i])
				quote = '\0';
			i++;
		}
		else if (line[i] == '|' && quote == '\0')
		{
			i++;
			while (line[i] && is_space(line[i]))
				i++;
			if (line[i] == '|')
				return (1); // syntax error: unquoted ||
		}
		else
			i++;
	}
	return (0); // no syntax error found
}
int chech_syntax(t_comand *check, char *line, t_data *data)
{
	if (!check)
		return(0);
	if (check_syntax(check))
	{
		free(line);
		data->last_exit_status = EXIT_SYNTAX_ERROR;
		return (0);
	}
	return (1);
}

int unclosed_quote(const char *str)
{
	int i = 0;
	char current_quote = 0;

	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '\"'))
		{
			if (!current_quote)
				current_quote = str[i]; // دخلنا فـ quote
			else if (current_quote == str[i])
				current_quote = 0; // خرجنا من quote
		}
		i++;
	}
	return (current_quote != 0);
}

int check_syntax(t_comand *tokens)
{
	t_comand *tmp;

	tmp = tokens;
	if(tmp->type == T_PIPE)
	{
		printf("syntax error near unexpected token `|'\n");
		return (1);
	}
	while (tmp)
	{
		if (tmp->type == T_PIPE)
		{
			if (!tmp->next || tmp->next->type == T_PIPE)
			{
				printf("syntax error near unexpected token `|'\n");
				return (1);
			}
		}
		if(tmp->type >= T_REDIR_IN && tmp->type <= T_HEREDOC)
		{
			if (!tmp->next || tmp->next->type != T_WORD)
			{
				printf(" syntax error near unexpected token (redirection)\n");
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}

char *generate_tmp_filename(t_data *data)
{
    static int  i;
    char *index_str;
    char *file_name;

	i = 0;
    index_str = ft_itoa(i, data);
    file_name = ft_strjoin(".heredoc_tmp_", index_str, data);
    gc_free_ptr(index_str, data);
    i++;
    return (file_name);
}
int open_file(char **filename, t_data *data)
{
	int fd;
	
	*filename = generate_tmp_filename(data);
	fd = open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		gc_free_all(data);
		exit (1);
	}
	return (fd);
}

