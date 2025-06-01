/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:01:21 by aaboudra          #+#    #+#             */
/*   Updated: 2025/05/31 17:20:40 by elben-id         ###   ########.fr       */
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

char	*ft_itoa(int n)
{
	unsigned int	nb;
	int				lent;
	char			*res;

	lent = len(n);
	res = (char *)gc_malloc(sizeof(char) * (lent + 1));
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

char *handle_quotes(char *token)
{
    int     i;
	int		j;
    char    quote;
    char   *new_token;

	i = 0;
	j = 0;
	quote = 0;
    new_token = gc_malloc(ft_strlen(token) + 1);
    while (token[i])
    {
        if ((token[i] == '\'' || token[i] == '"') && quote == 0)
        {
            quote = token[i];
            i++;
        }
        else if (token[i] == quote)
        {
            quote = 0;
            i++;
        }
        else
            new_token[j++] = token[i++];
    }
    new_token[j] = '\0';
	gc_free_ptr(token);
    return (new_token);
}


int ft_atoi(const char *str)
{
	int i = 0;
	int neg_flag = 1;
	int res = 0;
	if(str[i]=='-')
	{
		if(str[i+1]=='+')
			return (0);
		neg_flag = -1;
		i++;
	}	
	if((str[i]<48 ||  str[i]>57) && str[i] != '+')
		return (0);
	if(str[i]=='+')
		i++;
	while ((str[i] != '\0') && (str[i] >= '0') && (str[i] <= '9'))
	{
		res = res * 10 + (str[i] - 48);
		i++;
	}
	res *= neg_flag;
	return(res);
}
