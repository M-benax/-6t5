/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 00:35:04 by aaboudra          #+#    #+#             */
/*   Updated: 2025/05/24 22:55:07 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void tokens_4(char *input, int *i)
{
    char quote;
    while (input[*i] && !is_s_char(input[*i]) && !is_space(input[*i]))
    {
        if (input[*i] == '\'' || input[*i] == '\"')
        {
            quote = input[(*i)++];
            while (input[*i] && input[*i] != quote)
                (*i)++;
            if (input[*i])
                (*i)++;
        }
        else 
            (*i)++;
    }
}

static void tokens_3(char *input,int *i)
{
    if((input[*i] == '<' || input[*i] == '>')
         && input[*i] == input[(*i + 1)])
            *i += 2;
        else
            (*i)++;
}

static void tokens_2(char quote,char *input,int *i)
{
    while (input[*i] && input[*i] != quote)
        (*i)++;
    if(input[*i] == quote)
        (*i)++;
}

char *tokens_1(char *input, int *i)
{
    int start;
    char quote;
    char *tocken;
    
    start = *i;
    if (input[*i] == '\'' || input[*i] == '\"')
    {
        quote = input[(*i)++];
        tokens_2(quote,input,i);
    }
    else if (is_s_char(input[*i]))
        tokens_3(input, i);
    else
        tokens_4(input, i);
    tocken = ft_strndup(input + start, *i - start);
    tocken = handle_quotes(tocken);
    return (tocken);
}

t_comand  *tokens(char *input)
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
        node = new_node(tokens_1(input, &i));
        if (!head)
            head = node;
        else
            current->next = node;
        current = node;
    }
    return (head);
}


