#include "../minishell.h"

// Simplified expand_variable, actual expansion is complex (handles quotes, $?, etc.)
// This version only handles simple $VAR and ${VAR}
static char *expand_one_variable(const char *var_str, t_env *env_list, int *error_flag)
{
    char    *var_name;
    char    *var_value;
    char    *expanded_val;
    int     len;
    int     i;

    *error_flag = 0;
    if (!var_str || var_str[0] != '$') return (ft_strdup(var_str));

    if (var_str[1] == '{') // ${VAR}
    {
        i = 2;
        while (var_str[i] && var_str[i] != '}')
            i++;
        if (var_str[i] != '}') { *error_flag = 1; return (ft_strdup(var_str)); } // Bad substitution
        len = i - 2;
        if (len == 0) {*error_flag = 1; return (ft_strdup(var_str));} // ${}
        var_name = ft_strndup(var_str + 2, len);
    }
    else if (var_str[1] == '?') // $?
    {
        // This needs access to data->last_exit_status, so expand_all_commands
        // should handle $? specifically or pass data down.
        // For now, let's assume expand_all_commands handles $? directly.
        // This function won't handle $? directly to keep it simpler.
         return (ft_strdup(var_str)); // Let outer function handle $?
    }
    else // $VAR
    {
        i = 1;
        // Valid var name chars: alpha, digit (not first), underscore
        if (!ft_isalpha(var_str[i]) && var_str[i] != '_')
            return (ft_strdup(var_str)); // Not a valid var start after $
        while (var_str[i] && (ft_isalnum(var_str[i]) || var_str[i] == '_'))
            i++;
        len = i - 1;
        if (len == 0) return (ft_strdup("$")); // Just "$"
        var_name = ft_strndup(var_str + 1, len);
    }

    if (!var_name) { *error_flag = 1; return (ft_strdup(var_str));} // Malloc error
    var_value = get_env_value(env_list, var_name); // Returns "" if not found, or value
    gc_free_ptr(var_name);

    // var_value from get_env_value is already gc_malloc'd if ft_strdup is used there
    // If get_env_value returns a direct pointer or "" literal, need to strdup here
    expanded_val = ft_strdup(var_value); // Ensure it's a new gc_malloc'd string
    // If get_env_value was mallocing, free that original var_value if it's not the same as expanded_val
    // but if get_env_value uses gc_malloc, it's fine.
    // For simplicity, assume get_env_value's result is okay to be used or ft_strdup'd
    
    return (expanded_val);
}

// Iterates through a string, finds $ occurrences not in single quotes, and expands them.
// This is a very simplified version. Real bash expansion is much more complex.
static char *expand_string_variables(char *str, t_data *data_ptr, int *error_flag)
{
    // TODO: Implement robust string expansion considering:
    // 1. Single quotes: no expansion inside ''.
    // 2. Double quotes: expansion happens, except for some special cases.
    // 3. $?, $VAR, ${VAR}, $ (literal if followed by space/null/quote).
    // 4. Concatenation of expanded parts and literal parts.
    // This placeholder just does a simple full string expansion for demo.
    char *temp_expanded;
    int local_error = 0;

    // Simple case: if the whole string starts with $, try to expand it
    if (str && str[0] == '$')
    {
        if (str[1] == '?')
        {
             temp_expanded = ft_itoa(data_ptr->last_exit_status);
        }
        else
        {
            temp_expanded = expand_one_variable(str, data_ptr->tenv, &local_error);
        }
        if (local_error) *error_flag = 1;
        gc_free_ptr(str); // Free original string
        return (temp_expanded); // Return new expanded string
    }
    return (str); // Return original if no expansion attempted here
}


static int expand_args_for_command(t_cmd *cmd_node, t_data *data_ptr)
{
    int i;
    int error_flag;

    if (!cmd_node || !cmd_node->args) return (0);
    i = 0;
    error_flag = 0;
    while (cmd_node->args[i])
    {
        // Expansion should not happen for delimiter of heredoc
        // This needs to be handled by parse_command or by checking redirection types
        // For now, assume all args are candidates for expansion.
        // TODO: Skip expansion for heredoc delimiters.
        // TODO: Handle tilde expansion ~ at start of word.

        // A more robust way would be to iterate through the string,
        // identify parts to expand, and rebuild the string.
        // The current expand_string_variables is too simple.
        cmd_node->args[i] = expand_string_variables(cmd_node->args[i], data_ptr, &error_flag);
        if (error_flag)
        {
            // Bash might print "bad substitution" or set error status
            display_error_message(cmd_node->args[0] ? cmd_node->args[0] : "expansion",
                                  cmd_node->args[i], "bad substitution");
            data_ptr->last_exit_status = EXIT_GENERAL_ERROR;
            return (1); // Error occurred
        }
        i++;
    }
    // TODO: After expansion, if an arg becomes empty due to expanding an unset var,
    // bash might remove that arg (field splitting) or treat it as an empty string.
    // This also involves quote removal.
    // For now, we keep empty strings if expansion results in one.
    // And then word splitting / field splitting happens after expansion and quote removal
    // which can result in more args or removal of empty args. Very complex.
    // Minishell subject does not require full field splitting, but quote removal
    // and basic expansion.
    return (0); // Success
}

int expand_all_commands(t_data *data_ptr)
{
    t_cmd *current_cmd;

    current_cmd = data_ptr->com;
    while (current_cmd)
    {
        // Expand arguments
        if (expand_args_for_command(current_cmd, data_ptr) != 0)
            return (1); // Error propagated, exit status set

        // Expand filenames in redirections (after quote removal by parser if any)
        // This is tricky: expansion in filenames happens, but not for heredoc delimiters.
        if (current_cmd->in_type != T_HEREDOC && current_cmd->in_file)
        {
            int err_f = 0;
            current_cmd->in_file = expand_string_variables(current_cmd->in_file, data_ptr, &err_f);
            if(err_f) {data_ptr->last_exit_status = 1; return 1;}

        }
        if (current_cmd->out_file)
        {
             int err_f = 0;
            current_cmd->out_file = expand_string_variables(current_cmd->out_file, data_ptr, &err_f);
            if(err_f) {data_ptr->last_exit_status = 1; return 1;}
        }
        current_cmd = current_cmd->next;
    }
    return (0); // Success
}
