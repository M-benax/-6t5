/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:01:21 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/13 19:31:30 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	len(int n)
{
	int	len;

	if (n == 0)
		return (1);
	len = 0;
	if (n < 0)
	{
		len++;
	}
	while (n)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static int	seg(int n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

char	*ft_itoa(int n, t_data *data)
{
	unsigned int	nb;
	int				lent;
	char			*res;

	lent = len(n);
	res = (char *)gc_malloc(sizeof(char) * (lent + 1), data);
	res[lent] = '\0';
	nb = seg(n);
	if (nb == 0)
	{
		res[0] = '0';
		return (res);
	}
	if (n < 0)
		res[0] = '-';
	while (nb > 0)
	{
		res[--lent] = (nb % 10) + '0';
		nb /= 10;
	}
	return (res);
}

t_comand *handle_quotes(char *token, t_data *data)
{
    int     i;
	int		j;
    char    quote;
    char   *new_token;
	int qouted;
	int has_unquoted_content;

	i = 0;
	j = 0;
	qouted = 0;
	quote = 0;
	has_unquoted_content = 0;
    new_token = gc_malloc(ft_strlen(token) + 1, data);
    while (token[i])
    {
        if ((token[i] == '\'' || token[i] == '"') && quote == 0)
        {
            quote = token[i];
			if (quote == '\'')
				qouted = 1;
			else
				qouted = 2;
            i++;
        }
        else if (token[i] == quote)
        {
			quote = 0;
            i++;
        }
        else
        {
            if (quote == 0)
                has_unquoted_content = 1;
            new_token[j++] = token[i++];
        }
    }
    new_token[j] = '\0';
	gc_free_ptr(token, data);
    if (has_unquoted_content)
        qouted = 0;
    return (new_node(new_token, qouted, data));
}


