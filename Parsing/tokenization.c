/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 00:35:04 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/16 19:42:40 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char  *collect_token(char *input, int *i, t_data *data)
{
    char quote = 0;
    int start = *i;

    while (input[*i] && !is_space(input[*i]) && !is_s_char(input[*i]))
    {
        if (input[*i] == '\'' || input[*i] == '\"')
        {
            quote = input[(*i)++];
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

    // Skip spaces
    while (input[*i] && is_space(input[*i]))
        (*i)++;
    if (input[*i] == '\0')
        return NULL;
    if (is_s_char(input[*i])) // < > | ...
    {
        start = *i;
        (*i)++;
        if (input[start] == input[*i]) // like <<
            (*i)++;
        token = ft_strndup(input + start, *i - start, data);
    }
    else
        token = collect_token(input, i, data); // هادي كتجمع token واحد بصح
    return handle_quotes(token, data); // تزول quotes إلى بغيتي
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


