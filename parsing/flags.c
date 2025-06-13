#include "../minishell.h"

// Renamed chek_type to a more descriptive name
static t_token_type get_token_type_from_string(const char *word)
{
    if (!word) return T_WORD; // Should not happen if tokenization is correct
    if (ft_strcmp(word, "<") == 0) return T_REDIR_IN;
    if (ft_strcmp(word, "<<") == 0) return T_HEREDOC;
    if (ft_strcmp(word, ">") == 0) return T_REDIR_OUT;
    if (ft_strcmp(word, ">>") == 0) return T_APPEND;
    if (ft_strcmp(word, "|") == 0) return T_PIPE;
    return T_WORD; // Default
}

void put_type(t_comand *token_node)
{
    if (token_node && token_node->word)
    {
        token_node->type = get_token_type_from_string(token_node->word);
    }
    // else: handle error or assume token_node and word are always valid
}
