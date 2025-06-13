#include "../minishell.h"

// Ensure set_env_var is defined, possibly in this file if not elsewhere
// or included from a shared env_utils.c
void set_env_var(t_env **env_list, char *var_name, char *var_value)
{
    t_env *current;
    t_env *prev;

    current = *env_list;
    prev = NULL;
    while (current) {
        if (current->var && ft_strcmp(current->var, var_name) == 0) {
            gc_free_ptr(current->val); // Free old value
            current->val = ft_strdup(var_value); // gc_malloc new one
            if (!current->val && var_value) { /* Malloc error */ }
            return;
        }
        prev = current;
        current = current->next;
    }
    // Var not found, add new node
    current = new_env_node(var_name, var_value); // Assumes new_env_node gc_mallocs
    if (!current) { /* Malloc error */ return; }
    if (!prev) // List was empty
        *env_list = current;
    else
        prev->next = current;
}


int builtin_cd(char **args, t_env **env_list)
{
    char *target_path;
    char old_pwd_buffer[PATH_MAX];
    char new_pwd_buffer[PATH_MAX];
    char *path_from_env;
    int  ret_status;

    path_from_env = NULL;
    ret_status = 0;
    if (!args[1] || (args[1][0] == '~' && args[1][1] == '\0') ) // cd or cd ~
    {
        target_path = get_env_value(*env_list, "HOME");
        if (!target_path || target_path[0] == '\0') {
            display_error_message("cd", NULL, "HOME not set");
            free(target_path); // if get_env_value mallocs
            return (1);
        }
        path_from_env = target_path; // To free it later
    } else if (args[2]) {
        display_error_message("cd", NULL, "too many arguments");
        return (1);
    } else
        target_path = args[1];

    old_pwd_buffer[0] = '\0';
    if (getcwd(old_pwd_buffer, PATH_MAX) == NULL) {
         display_error_message("cd", "getcwd for OLDPWD", strerror(errno));
         // Not fatal for cd itself, but OLDPWD won't be set correctly
    }

    if (chdir(target_path) != 0) {
        display_error_message("cd", target_path, strerror(errno));
        ret_status = 1;
    } else {
        if (getcwd(new_pwd_buffer, PATH_MAX) == NULL) {
            display_error_message("cd", "getcwd for PWD", strerror(errno));
            ret_status = 1; // chdir succeeded but couldn't get new PWD
        } else {
            if (old_pwd_buffer[0] != '\0')
                set_env_var(env_list, "OLDPWD", old_pwd_buffer);
            set_env_var(env_list, "PWD", new_pwd_buffer);
        }
    }
    free(path_from_env); // Free if allocated by get_env_value
    return (ret_status);
}