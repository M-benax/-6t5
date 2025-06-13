#include "../minishell.h"

static void cleanup_after_command_cycle(t_data *data, t_comand *token_list, char *line)
{
    gc_free_cmd_structure(data->com);
    data->com = NULL;
    gc_free_token_list(token_list); // Assumes token strings are handled by gc or parse_command
    free(line);
}

int main(int ac, char **av, char **env)
{
    char        *line;
    t_comand    *token_list;
    t_data      data_ptr;
    t_data      *data = &data_ptr;

    (void)ac;
    (void)av;
    data->tenv = init_env(env);
    data->com = NULL;
    data->last_exit_status = 0;
    if (!data->tenv && env && env[0]) /* Critical error if env can't be initialized but env was provided*/
    {
        write(STDERR_FILENO, "minishell: critical error initializing environment\n", 51);
        return (EXIT_FAILURE);
    }
    while (1)
    {
        signal_init();
        line = readline("minishell-$ ");
        if (!line)
        {
            write(STDOUT_FILENO, "exit\n", 5);
            gc_free_all();
            exit(data->last_exit_status);
        }
        if (line[0] != '\0')
            add_history(line);
        else
        {
            free(line);
            continue;
        }
        token_list = tokens(line);
        if (check_syntax(token_list) != 0)
        {
            data->last_exit_status = EXIT_SYNTAX_ERROR;
            cleanup_after_command_cycle(data, token_list, line);
            continue;
        }
        if (!token_list)
        {
             cleanup_after_command_cycle(data, token_list, line);
             continue;
        }
        gc_free_cmd_structure(data->com); // Free previous commands before parsing new
        data->com = parse_command(token_list);
        // gc_free_token_list(token_list); // Moved to cleanup_after_command_cycle

        if (data->com)
        {
            if (expand_all_commands(data) != 0)
            {
                // last_exit_status is set by expand_all_commands on error
                cleanup_after_command_cycle(data, token_list, line);
                continue;
            }
            start_execution(data);
        }
        cleanup_after_command_cycle(data, token_list, line);
    }
    return (data->last_exit_status); // Should not be reached
}