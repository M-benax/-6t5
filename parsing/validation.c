#include "../minishell.h"

// Placeholder: Actual unclosed quote check needs more robust logic,
// especially considering quotes within words.
static int has_unclosed_quotes(const char *str)
{
    int single_quotes = 0;
    int double_quotes = 0;
    int i = 0;

    while (str && str[i])
    {
        if (str[i] == '\'')
            single_quotes++;
        else if (str[i] == '"')
            double_quotes++;
        i++;
    }
    return ((single_quotes % 2 != 0) || (double_quotes % 2 != 0));
}

// Returns 0 on success, non-zero on syntax error
int check_syntax(t_comand *tokens)
{
    t_comand *current;

    current = tokens;
    if (!current) return (0); // No tokens, no syntax error

    // Rule: Pipe cannot be the first or last token, or be followed by another pipe.
    if (current->type == T_PIPE) {
        display_error_message(NULL, NULL, "syntax error near unexpected token `|'");
        return (EXIT_SYNTAX_ERROR);
    }

    while (current)
    {
        // Rule: Redirection operators must be followed by a T_WORD (filename)
        if (current->type >= T_REDIR_IN && current->type <= T_HEREDOC)
        {
            if (!current->next || current->next->type != T_WORD)
            {
                display_error_message(NULL, NULL, "syntax error near unexpected token (redirection)");
                return (EXIT_SYNTAX_ERROR);
            }
             if (has_unclosed_quotes(current->next->word)) // Check filename for unclosed quotes
            {
                 display_error_message(NULL, current->next->word, "ambiguous redirect or unclosed quote");
                 return (EXIT_SYNTAX_ERROR);
            }
        }
        // Rule: Pipe cannot be followed by another pipe or be the last token.
        if (current->type == T_PIPE)
        {
            if (!current->next || current->next->type == T_PIPE)
            {
                display_error_message(NULL, NULL, "syntax error near unexpected token `|'");
                return (EXIT_SYNTAX_ERROR);
            }
        }
        // Rule: Check for unclosed quotes in T_WORD tokens
        // This is a simplistic check; proper parsing should handle quoted segments.
        if (current->type == T_WORD && has_unclosed_quotes(current->word))
        {
            display_error_message(NULL, NULL, "syntax error (unclosed quote)");
            return (EXIT_SYNTAX_ERROR);
        }
        current = current->next;
    }
    return (0); // Syntax is OK
}

// generate_tmp_filename and open_file are used by heredoc logic in parse.c
// They should not be static if called from parse.c and defined in validation.c
// Unless heredoc handling is also moved to validation.c

char *generate_tmp_filename(void) // Not static
{
    static int  heredoc_count = 0;
    char        *count_str;
    char        *filename;
    char        *tmp_prefix = "/tmp/.minishell_heredoc_"; // Use /tmp for temporary files

    count_str = ft_itoa(heredoc_count++);
    if (!count_str) return (NULL);

    filename = ft_strjoin(tmp_prefix, count_str);
    free(count_str); // ft_itoa typically mallocs
    // gc_free_ptr(count_str); // if ft_itoa uses gc_malloc
    return (filename); // This filename needs to be gc_malloc'd or freed by caller
}

int open_file(char **filename_ptr) // Not static; takes char** to return allocated filename
{
    int fd;

    *filename_ptr = generate_tmp_filename();
    if (!*filename_ptr)
    {
        perror("minishell: heredoc: failed to generate temp filename");
        return (-1);
    }
    // Use ft_strdup to make it gc_managed if generate_tmp_filename uses malloc
    // char *gc_filename = ft_strdup(*filename_ptr);
    // free(*filename_ptr);
    // *filename_ptr = gc_filename;


    fd = open(*filename_ptr, O_CREAT | O_WRONLY | O_TRUNC, 0600); // Permissions 0600 for private temp file
    if (fd == -1)
    {
        perror(*filename_ptr);
        gc_free_ptr(*filename_ptr); // Free if open fails
        *filename_ptr = NULL;
    }
    return (fd);
}