#include "../minishell.h"

// get_var and get_val might be defined in init_tenv.c or shared env_utils.c
// Ensure they are prototyped in minishell.h if used here and defined elsewhere.

// Assuming is_valid_identifier_char and is_valid_identifier_start are helpers
static int is_valid_identifier_char(char c, int is_start)
{
    if (ft_isalpha(c) || c == '_')
        return (1);
    if (!is_start && ft_isalnum(c)) // Digits allowed after first char
        return (1);
    return (0);
}

static int is_valid_export_key(const char *key_str)
{
    int i;

    if (!key_str || !is_valid_identifier_char(key_str[0], 1)) // Must start with letter or _
        return (0);
    i = 1;
    while (key_str[i])
    {
        if (!is_valid_identifier_char(key_str[i], 0))
            return (0);
        i++;
    }
    return (1);
}


// Modified to take t_data and return int status
// handel_export should be the function name used in builtins_core.c
int handel_export(t_data *data_ptr) // Renamed from 'command' to 'data_ptr'
{
    char    **args;
    int     i;
    char    *arg_str;
    char    *var_name;
    char    *var_value;
    char    *equal_sign_ptr;
    int     ret_status;

    if (!data_ptr || !data_ptr->com || !data_ptr->com->args)
        return (EXIT_GENERAL_ERROR); // Should not happen

    args = data_ptr->com->args;
    ret_status = 0; // Success by default
    i = 1; // Start from args[1]
    while (args[i])
    {
        arg_str = args[i];
        equal_sign_ptr = ft_strchr(arg_str, '=');
        if (equal_sign_ptr) // Case: export VAR=value
        {
            var_name = ft_strndup(arg_str, equal_sign_ptr - arg_str);
            var_value = ft_strdup(equal_sign_ptr + 1);
        }
        else // Case: export VAR (value will be NULL or not set)
        {
            var_name = ft_strdup(arg_str);
            var_value = NULL; // Or some way to signify no value vs empty string
        }

        if (!var_name || (equal_sign_ptr && !var_value && *(equal_sign_ptr+1) != '\0' ) ) // Malloc error check
        {
            gc_free_ptr(var_name); gc_free_ptr(var_value);
            ret_status = EXIT_GENERAL_ERROR; i++; continue;
        }

        if (!is_valid_export_key(var_name))
        {
            display_error_message("export", arg_str, "not a valid identifier");
            ret_status = EXIT_GENERAL_ERROR;
        }
        else
        {
            // Use set_env_var from builtin_cd.c or a shared env_util
            // It handles both adding new and updating existing.
            // If var_value is NULL from here, set_env_var should handle it
            // (e.g., by setting env->val to NULL, or to an empty string if strdup(NULL) is "" )
            // For export VAR (no =), bash creates var with no value initially.
            // If export VAR= (empty value), bash creates var with empty string value.
            if (var_value == NULL && !equal_sign_ptr) // export VAR
                 set_env_var(&(data_ptr->tenv), var_name, NULL); // Special case for no value
            else if (var_value == NULL && equal_sign_ptr) // export VAR= (empty string)
                 set_env_var(&(data_ptr->tenv), var_name, "");
            else
                 set_env_var(&(data_ptr->tenv), var_name, var_value);
        }
        gc_free_ptr(var_name);
        gc_free_ptr(var_value);
        i++;
    }
    return (ret_status);
}