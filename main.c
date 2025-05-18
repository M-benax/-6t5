
#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;

    t_env *env = NULL;
    init_env(&env, envp);

    t_cmd cmd;
    char *args[] = {"/bin/ls", NULL};

    cmd.args = args;
    cmd.in_type = 0;
    cmd.in_file = NULL;
    cmd.out_type = T_WORD; // <= مهمّة!
    cmd.out_file = NULL;
    cmd.next = NULL;

    execute_cmd(&cmd, env);

    free_env(env);
    return (0);
}
