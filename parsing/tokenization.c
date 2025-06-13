#include "../minishell.h" // أو المسار الصحيح

// افترض أن is_s_char و is_space معرّفتان في minishell.h (من utils.c)
// وأن new_node و handle_quotes معرّفتان أيضًا

static void tokens_4(char *input, int *i) // word or quoted string
{
    char quote_char;

    while (input[*i] && !is_s_char(input[*i]) && !is_space(input[*i]))
    {
        if (input[*i] == '\'' || input[*i] == '"')
        {
            quote_char = input[*i];
            (*i)++; // Skip opening quote
            while (input[*i] && input[*i] != quote_char)
                (*i)++;
            if (input[*i] == quote_char) // Skip closing quote
                (*i)++;
        }
        else
            (*i)++;
    }
}

static void tokens_3(char *input, int *i) // special char or double special char
{
    if ((input[*i] == '<' || input[*i] == '>') && input[*i] == input[*i + 1])
        *i += 2; // For >> or <<
    else
        (*i)++; // For | > <
}

// tokens_2 (إذا كانت تستخدم لـ single/double quotes بشكل منفصل، قد تكون مدمجة في tokens_4)
// static void tokens_2(char quote_char, char *input, int *i)
// {
//     while (input[*i] && input[*i] != quote_char)
//         (*i)++;
//     if (input[*i] == quote_char)
//         (*i)++; // consume closing quote
// }


// tokens_1: extracts one token
char *extract_one_token_string(char *input, int *i) // غيرت الاسم ليكون أوضح
{
    int     start_index;
    char    *token_str;
    // char    quote_char; // Not directly used here anymore if tokens_4 handles it

    start_index = *i;
    if (is_s_char(input[*i])) // Handle <, >, |, <<, >>
        tokens_3(input, i);
    else // Handle words and quoted strings
        tokens_4(input, i);

    token_str = ft_strndup(input + start_index, *i - start_index);
    // handle_quotes is usually applied *after* tokenization, during parsing/expansion
    // If you apply it here, ensure it's what you intend for early processing.
    // token_str = handle_quotes(token_str); // Maybe not here
    return (token_str);
}


t_comand *tokens(char *input)
{
    t_comand    *head;
    t_comand    *current;
    t_comand    *new_token_node;
    char        *token_str;
    int         i;

    if (!input || !input[0])
        return (NULL);
    head = NULL;
    current = NULL;
    i = 0;
    while (input[i])
    {
        while (input[i] && is_space(input[i]))
            i++;
        if (!input[i])
            break;
        token_str = extract_one_token_string(input, &i);
        if (!token_str) { /* Malloc error in ft_strndup */ break; }

        new_token_node = new_node(token_str); // new_node duplicates token_str
        if (!new_token_node) { /* Malloc error */ free(token_str); break; }
        // free(token_str); // Free the temporary token_str if new_node duplicates it

        put_type(new_token_node); // Set the type of the token

        if (!head)
            head = new_token_node;
        else
            current->next = new_token_node;
        current = new_token_node;
    }
    return (head);
}