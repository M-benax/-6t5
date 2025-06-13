#include "../minishell.h" // أو المسار الصحيح لملف minishell.h

// --- Static Helper Function Prototypes for this file (if any complex ones) ---
static t_cmd *init_cmd_node(void);
static void add_arg_to_cmd(t_cmd *cmd_node, char *word_token);
static int handle_heredoc_input(const char *limiter, int write_fd);
static void process_token_for_command(t_comand **current_token_ptr,
                                      t_cmd *current_cmd_node,
                                      t_data *data); // data can be NULL

// --- Function Definitions ---

static int handle_heredoc_input(const char *limiter, int write_fd)
{
    char    *line;
    int     original_exit_status; // To restore if Ctrl+C happens (TODO: implement fully)

    (void)original_exit_status; // Suppress unused variable warning for now

    // TODO: Implement specific signal handling for heredoc (Ctrl+C should break,
    //       Ctrl+D on its own line should be treated as EOF for heredoc input)
    // For now, readline handles Ctrl+D as NULL return.
    // Ctrl+C will currently use the main interactive handler.

    while (1)
    {
        line = readline("> "); // Heredoc prompt
        if (!line) // EOF (Ctrl+D)
        {
            // Bash prints: "minishell: warning: here-document delimited by end-of-file (wanted `delimiter')"
            // display_error_message is a good candidate here if it's globally available
            // For now, using write directly for simplicity if display_error_message isn't available at this stage
            write(STDERR_FILENO, "minishell: warning: here-document at line ", 43); // Line number is tricky
            write(STDERR_FILENO, " delimited by end-of-file (wanted `", 35);
            write(STDERR_FILENO, limiter, ft_strlen(limiter));
            write(STDERR_FILENO, "')\n", 3);
            return (1); // Indicate premature termination or error
        }
        if (ft_strcmp(line, limiter) == 0)
        {
            free(line);
            break;
        }
        write(write_fd, line, ft_strlen(line));
        write(write_fd, "\n", 1);
        free(line);
    }
    return (0); // Success
}

static t_cmd *init_cmd_node(void)
{
    t_cmd *node;

    node = gc_malloc(sizeof(t_cmd));
    if (!node)
        return (NULL);
    node->args = NULL;
    node->in_file = NULL;
    node->out_file = NULL;
    node->in_type = 0; // Or some T_NONE enum value
    node->out_type = 0; // Or some T_NONE enum value
    node->next = NULL;
    return (node);
}

static void add_arg_to_cmd(t_cmd *cmd_node, char *word_token)
{
    int     i;
    char    **new_args_array;
    int     current_arg_count;

    current_arg_count = 0;
    if (cmd_node->args) // Count existing arguments
    {
        while (cmd_node->args[current_arg_count])
            current_arg_count++;
    }

    // Allocate for current args + new arg + NULL terminator
    new_args_array = gc_malloc(sizeof(char *) * (current_arg_count + 2));
    if (!new_args_array)
    {
        // TODO: Proper error handling if gc_malloc fails
        perror("minishell: gc_malloc failed in add_arg_to_cmd");
        return;
    }

    // Copy existing arguments
    i = 0;
    while (i < current_arg_count)
    {
        new_args_array[i] = cmd_node->args[i];
        i++;
    }

    // Add the new argument (strdup it, as token_word might be from a temp buffer or token list)
    new_args_array[i] = ft_strdup(word_token);
    if (!new_args_array[i] && word_token) // Check if ft_strdup failed
    {
        // TODO: Free new_args_array and handle error
        perror("minishell: ft_strdup failed in add_arg_to_cmd");
        gc_free_ptr(new_args_array); // Free the partially filled array
        return;
    }
    i++;
    new_args_array[i] = NULL; // Null-terminate the new array

    gc_free_ptr(cmd_node->args); // Free the old array of pointers (it's gc_managed)
    cmd_node->args = new_args_array;
}

// This function advances the token pointer (*current_token_ptr)
static void process_token_for_command(t_comand **current_token_ptr,
                                      t_cmd *current_cmd_node,
                                      t_data *data) // data can be NULL if not changing exit status here
{
    t_comand *token;
    char     *heredoc_temp_filename; // For heredoc temporary file name
    int      heredoc_fd;

    (void)data; // Suppress unused warning if data is not used for exit status
    token = *current_token_ptr;
    if (!token)
        return;

    if (token->type == T_WORD)
    {
        add_arg_to_cmd(current_cmd_node, token->word);
        *current_token_ptr = token->next; // Advance to next token
    }
    else if (token->type >= T_REDIR_IN && token->type <= T_HEREDOC)
    {
        if (!token->next || token->next->type != T_WORD)
        {
            // This should ideally be caught by check_syntax
            display_error_message(NULL, token->word, "syntax error: missing filename for redirection");
            if (data) data->last_exit_status = EXIT_SYNTAX_ERROR;
            // To stop further processing for this command group, we can advance ptr to NULL
            // Or the caller (parse_command) should check an error flag.
            // For now, let's assume parse_command handles error propagation if data is passed.
            // If data is NULL, we just consume tokens and let check_syntax handle user message.
            // To be safe, consume the problematic token and its "filename" if any.
            if (token->next) *current_token_ptr = token->next->next;
            else *current_token_ptr = token->next;
            return;
        }

        // Handle actual redirection assignment
        if (token->type == T_REDIR_IN) {
            current_cmd_node->in_type = T_REDIR_IN;
            gc_free_ptr(current_cmd_node->in_file); // Free previous if any
            current_cmd_node->in_file = ft_strdup(token->next->word);
        } else if (token->type == T_REDIR_OUT) {
            current_cmd_node->out_type = T_REDIR_OUT;
            gc_free_ptr(current_cmd_node->out_file);
            current_cmd_node->out_file = ft_strdup(token->next->word);
        } else if (token->type == T_APPEND) {
            current_cmd_node->out_type = T_APPEND;
            gc_free_ptr(current_cmd_node->out_file);
            current_cmd_node->out_file = ft_strdup(token->next->word);
        } else if (token->type == T_HEREDOC) {
            current_cmd_node->in_type = T_HEREDOC;
            gc_free_ptr(current_cmd_node->in_file);

            // open_file (from validation.c) allocates filename_ptr
            // and returns fd. filename_ptr points to a malloc'd string.
            heredoc_fd = open_file(&heredoc_temp_filename);
            if (heredoc_fd == -1) {
                if (data) data->last_exit_status = EXIT_GENERAL_ERROR;
                free(heredoc_temp_filename); // Free if open_file allocated it even on error
                *current_token_ptr = NULL; // Stop further processing
                return;
            }
            // Store a gc_managed copy of the temporary filename
            current_cmd_node->in_file = ft_strdup(heredoc_temp_filename);
            free(heredoc_temp_filename); // Free the malloc'd version from open_file

            if (handle_heredoc_input(token->next->word, heredoc_fd) != 0) {
                if (data) data->last_exit_status = 1; // Heredoc interrupted
                if (current_cmd_node->in_file) unlink(current_cmd_node->in_file); // Clean up temp file
                gc_free_ptr(current_cmd_node->in_file);
                current_cmd_node->in_file = NULL;
                // Stop processing tokens for this command if heredoc fails/is interrupted
                // This is tricky; how to signal outer loop?
                // For now, let execution handle missing in_file if heredoc fails.
            }
            close(heredoc_fd);
        }
        *current_token_ptr = token->next->next; // Consume redirection token AND its filename
    }
    else if (token->type == T_PIPE)
    {
        // Pipe is handled by the outer loop in parse_command, just advance past it
        *current_token_ptr = token->next;
    }
    else // Should not happen if token types are well-defined
    {
        *current_token_ptr = token->next;
    }
}


t_cmd *parse_command(t_comand *tokens_list)
{
    t_cmd       *cmd_list_head;
    t_cmd       *current_cmd_being_built;
    t_comand    *current_token_iterator;

    if (!tokens_list)
        return (NULL);

    cmd_list_head = NULL;
    current_cmd_being_built = NULL;
    current_token_iterator = tokens_list;

    // Always start with a command structure
    current_cmd_being_built = init_cmd_node();
    if (!current_cmd_being_built)
        return (NULL); // Malloc failure
    cmd_list_head = current_cmd_being_built;

    while (current_token_iterator)
    {
        if (current_token_iterator->type == T_PIPE)
        {
            // Before creating a new command for after the pipe,
            // check if the current command is empty (e.g., " | cmd" or "cmd | | cmd").
            // Such cases should be syntax errors caught by check_syntax.
            // If current_cmd_being_built has no args and no redirections, it's problematic.

            t_cmd *next_cmd_node = init_cmd_node();
            if (!next_cmd_node) { /* Malloc error, cleanup */ return (cmd_list_head); }
            current_cmd_being_built->next = next_cmd_node;
            current_cmd_being_built = next_cmd_node;
            current_token_iterator = current_token_iterator->next; // Consume the pipe token

            if (!current_token_iterator) // Pipe at the very end of the line
            {
                // This is a syntax error, should be caught by check_syntax.
                // The created empty cmd_node will just be there.
                break;
            }
        }
        else // Process T_WORD or Redirection tokens
        {
            // process_token_for_command will advance current_token_iterator
            process_token_for_command(&current_token_iterator,current_cmd_being_built, NULL);
        }
    }
    return (cmd_list_head);
}