/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elben-id <elben-id@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 00:35:04 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/23 15:49:50 by elben-id         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char  *collect_token(char *input, int *i, t_data *data)
{
    int start;
    char quote;

    start = *i;
    while (input[*i] && !is_space(input[*i]) && !is_s_char(input[*i]))
    {
        if ((input[*i] == '\'' || input[*i] == '\"'))
        {
            quote = input[*i];
            if (input[*i + 1] == quote)
            {
                if (*i == start)
                {
                    *i += 2;
                    return ft_strndup(input + start, 2, data);  // return ''
                }
                else
                    break;
            }
            (*i)++;
            while (input[*i] && input[*i] != quote)
                (*i)++;
            if (input[*i] == quote)
                (*i)++;
        }
        else
            (*i)++;
    }
    return ft_strndup(input + start, *i - start, data);
}

t_comand *tokens_1(char *input, int *i, t_data *data)
{
    char *token;
    int start;

    while (input[*i] && is_space(input[*i]))
        (*i)++;
    if (input[*i] == '\0')
        return NULL;
    if (is_s_char(input[*i]))
    {
        start = *i;
        (*i)++;
        if (input[start] == input[*i])
            (*i)++;
        token = ft_strndup(input + start, *i - start, data);
    }
    else
        token = collect_token(input, i, data);
    return handle_quotes(token, data);
}

t_comand  *tokens(char *input, t_data *data)
{
    t_comand *head;
    t_comand *current;
    t_comand *node;
    int         i;

    if (!input || !input[0])
        return (NULL);
    head = NULL;
    current = NULL;
    i = 0;
    while (input[i])
    {
        while (is_space(input[i]))
            i++;
        if(!input[i])
            break;
        node = tokens_1(input, &i, data);
        
        if (!head)
            head = node;
        else
            current->next = node;
        current = node;
    }
    return (head);
}

