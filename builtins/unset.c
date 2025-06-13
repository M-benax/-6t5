#include "../minishell.h"

static void remove_env_var_node(t_env **env_list_ptr, const char *key_to_remove)
{
    t_env *current;
    t_env *prev;

    if (!env_list_ptr || !*env_list_ptr || !key_to_remove)
        return;
    current = *env_list_ptr;
    prev = NULL;
    while (current)
    {
        if (current->var && ft_strcmp(current->var, key_to_remove) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env_list_ptr = current->next; // Removing the head
            gc_free_ptr(current->var);
            gc_free_ptr(current->val);
            gc_free_ptr(current);
            return; // Found and removed
        }
        prev = current;
        current = current->next;
    }
}

// Simplified is_valid_var_name for unset (alphanum and _)
// Bash has more complex rules for variable names.
static int is_valid_unset_var_name(const char *str)
{
    int i;

    if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
        return 0; // Must start with letter or underscore
    i = 1;
    while(str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return 0;
        i++;
    }
    return 1;
}

// Returns 0 on success, 1 on error (e.g. invalid identifier)
int unset(char **args, t_env **env_list_ptr)
{
    int i;
    int overall_status; // To return error if any identifier was invalid

    if (!args || !env_list_ptr) return (1); // Should not happen

    overall_status = 0;
    i = 1; // Start from args[1]
    while (args[i])
    {
        if (!is_valid_unset_var_name(args[i]))
        {
            display_error_message("unset", args[i], "not a valid identifier");
            overall_status = 1; // Mark that an error occurred
        }
        else
        {
            remove_env_var_node(env_list_ptr, args[i]);
        }
        i++;
    }
    return (overall_status);
}