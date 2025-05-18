#include "minishell.h"

int ft_echo(t_cmd *cmd)
{
    int i = 1;
    int new_line = 1;

    if (cmd->args[i] && ft_strcmp(cmd->args[i], "-n") == 0)
    {
        new_line = 0;
        i++;
    }

    while (cmd->args[i])
    {
        write(1, cmd->args[i], ft_strlen(cmd->args[i]));
        if (cmd->args[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (new_line)
        write(1, "\n", 1);

    return 0;
}

int ft_cd(t_cmd *cmd, t_env *env)
{
    (void)env;

    if (!cmd->args[1] || chdir(cmd->args[1]) == -1)
    {
        perror("cd");
        return 1;
    }
    return (0);
}

int ft_pwd(void)
{
    char *cwd = getcwd(NULL, 0);
    if (cwd)
    {
        printf("%s\n", cwd);
        free(cwd);
    }
    else
        perror("pwd");
    return 0;
}

int ft_exit(t_cmd *cmd)
{
    int status = 0;
    if (cmd->args[1])
        status = ft_atoi(cmd->args[1]);

    exit(status);
}
int ft_env(t_env *env)
{
    while (env)
    {
        if (env->value)
            printf("%s=%s\n", env->key, env->value);
        env = env->next;
    }
    return 0;
}

int ft_unset(t_cmd *cmd, t_env **env)
{
    int i = 1;
    while (cmd->args[i])
    {
        unset_env(env, cmd->args[i]);
        i++;
    }
    return 0;
}

int ft_export(t_cmd *cmd, t_env **env)
{
    int i = 1;
    char *equal;

    while (cmd->args[i])
    {
        equal = ft_strchr(cmd->args[i], '=');
        if (equal)
        {
            char *key = ft_substr(cmd->args[i], 0, equal - cmd->args[i]);
            char *value = ft_strdup(equal + 1);
            unset_env(env, key);
            set_env(env, key, value);
            free(key);
            free(value);
        }
        i++;
    }
    return 0;
}