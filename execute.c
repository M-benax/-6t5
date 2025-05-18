#include "minishell.h"

void execute_cmd(t_cmd *cmd, t_env *env)
{
    if (run_builtin(cmd, &env) != -1)
        return;

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }
    if (pid == 0)
    {
        apply_redirections(cmd);
        execve(cmd->args[0], cmd->args, env_to_envp(env));
        perror("execve");
        exit(1); // Ensure the child process exits if execve fails
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
    }
}


