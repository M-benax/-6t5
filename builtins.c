#include "minishell.h"

int run_builtin(t_cmd *cmd, t_env **env)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        return -1;
    if (ft_strcmp(cmd->args[0], "echo") == 0)
        return ft_echo(cmd);
    if (ft_strcmp(cmd->args[0], "cd") == 0)
        return ft_cd(cmd, *env);
    if (ft_strcmp(cmd->args[0], "pwd") == 0)
        return ft_pwd();
    if (ft_strcmp(cmd->args[0], "exit") == 0)
        return ft_exit(cmd);
    if (ft_strcmp(cmd->args[0], "unset") == 0)
        return ft_unset(cmd, env);
    return -1;
}
