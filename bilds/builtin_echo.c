#include "../minishell.h"

int builtin_echo(char **args)
{
    int i;
    int n_option;

    i = 1;
    n_option = 0;
    if (args[i] && ft_strcmp(args[i], "-n") == 0)
    {
        n_option = 1;
        i++;
    }
    while (args[i])
    {
        write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
        if (args[i+1])
            write(STDOUT_FILENO, " ", 1);
        i++;
    }
    if (!n_option)
        write(STDOUT_FILENO, "\n", 1);
    return (0);
}