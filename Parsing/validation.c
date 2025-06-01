/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 23:25:36 by aaboudra          #+#    #+#             */
/*   Updated: 2025/05/26 18:38:06 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int chech_syntax(t_comand *check)
{
	if (!check)
		return(0);
	if (check_syntax(check))
	{
		printf("syntax erroe \n");
		return (0);
	}
	return (1);
}

static int  unclosed_quote(char *str)
{
	int i;
	int c_1;
	int c_2;
	
	i = 0;
	c_1 = 0;
	c_2 = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (str[i] == '\'')
				c_1++;
			else if (str[i] == '\"')
				c_2++;
		}
		i++;
	}
	if (c_1 % 2 != 0 || c_2 % 2 != 0)
		return (1);
	return (0);
}

int check_syntax(t_comand *tokens)
{
	t_comand *tmp;

	tmp = tokens;
	if(tmp->type == T_PIPE)
		return 1;
	while (tmp)
	{
		if (tmp->type == T_WORD && unclosed_quote(tmp->word))
			return (1);
		if (tmp->type == T_PIPE)
		{
			if (!tmp->next || tmp->next->type == T_PIPE)
				return (1);
		}
		if(tmp->type >= T_REDIR_IN && tmp->type <= T_HEREDOC)
		{
			if (!tmp->next || tmp->next->type != T_WORD)
				return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

char *generate_tmp_filename(void)
{
    static int  i;
    char *index_str;
    char *file_name;

	i = 0;
    index_str = ft_itoa(i);
    file_name = ft_strjoin(".heredoc_tmp_", index_str);
    gc_free_ptr(index_str);
    i++;
    return (file_name);
}
int open_file(char **filename)
{
	int fd;
	
	*filename = generate_tmp_filename();
	fd = open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		gc_free_all();
		exit (1);
	}
	return (fd);
}

