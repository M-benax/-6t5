#include "../minishell.h"

void set_env_var(t_env **env_list, char *var_name, char *var_value)
{
    t_env *current;
    t_env *new_node_env;
    t_env *prev;

    current = *env_list;
    prev = NULL;
    while (current)
    {
        if (ft_strcmp(current->var, var_name) == 0)
        {
            gc_free_ptr(current->val);
            current->val = ft_strdup(var_value);
            return;
        }
        prev = current;
        current = current->next;
    }
    new_node_env = new_env_node(var_name, var_value);
    if (!*env_list)
        *env_list = new_node_env;
    else
        prev->next = new_node_env;
}

int builtin_cd(char **args, t_env **env_list)
{
    char *target_path;
    char old_pwd_buffer[PATH_MAX];
    char new_pwd_buffer[PATH_MAX];
    char *path_from_env;

    path_from_env = NULL;
    if (!args[1])
    {
        target_path = get_env_value(*env_list, "HOME");
        if (!target_path || target_path[0] == '\0')
        {
            write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
            free(target_path);
            return (1);
        }
        path_from_env = target_path;
    }
    else if (args[2])
    {
        write(STDERR_FILENO, "minishell: cd: too many arguments\n", 34);
        return (1);
    }
    else
        target_path = args[1];
    old_pwd_buffer[0] = '\0';
    if (getcwd(old_pwd_buffer, PATH_MAX) == NULL) {}
    if (chdir(target_path) != 0)
    {
        perror(target_path);
        free(path_from_env);
        return (1);
    }
    free(path_from_env);
    if (getcwd(new_pwd_buffer, PATH_MAX) == NULL)
    {
        perror("minishell: cd: getcwd error after chdir");
        return (1);
    }
    if (old_pwd_buffer[0] != '\0')
        set_env_var(env_list, "OLDPWD", old_pwd_buffer);
    set_env_var(env_list, "PWD", new_pwd_buffer);
    return (0);
}