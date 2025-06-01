#include "../minishell.h"

int builtin_pwd(void)
{
    char current_path_buffer[PATH_MAX];

    if (getcwd(current_path_buffer, PATH_MAX) == NULL)
    {
        perror("minishell: pwd: getcwd");
        return (1);
    }
    write(STDOUT_FILENO, current_path_buffer, ft_strlen(current_path_buffer));
    write(STDOUT_FILENO, "\n", 1);
    return (0);
}