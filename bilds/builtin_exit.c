#include "../minishell.h"

int is_str_numeric(const char *str)
{
    int i;

    if (!str || *str == '\0')
        return (0);
    i = 0;
    if (str[i] == '+' || str[i] == '-')
        i++;
    if (str[i] == '\0')
        return (0);
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

int builtin_exit(char **args, t_data *data)
{
    int exit_code_val;
    (void)data; // لمنع تحذير unused parameter إذا كانت gc_free_all لا تعتمد عليه مباشرة

    printf("[DEBUG builtin_exit] Entered.\n");
    write(STDOUT_FILENO, "exit\n", 5);
    exit_code_val = get_global_exit_status();
    printf("[DEBUG builtin_exit] Initial exit_code_val from $?: %d\n", exit_code_val);

    if (args[1])
    {
        printf("[DEBUG builtin_exit] Argument 1: %s\n", args[1]);
        if (is_str_numeric(args[1]))
        {
            exit_code_val = ft_atoi(args[1]) % 256;
            if (exit_code_val < 0)
                exit_code_val += 256;
            printf("[DEBUG builtin_exit] Numeric arg, new exit_code_val: %d\n", exit_code_val);
        }
        else
        {
            write(STDERR_FILENO, "minishell: exit: ", 17);
            write(STDERR_FILENO, args[1], ft_strlen(args[1]));
            write(STDERR_FILENO, ": numeric argument required\n", 28);
            exit_code_val = 2;
            printf("[DEBUG builtin_exit] Non-numeric arg, exiting with: %d\n", exit_code_val);
            gc_free_all();
            // rl_clear_history();
            exit(exit_code_val);
        }
        if (args[2])
        {
             printf("[DEBUG builtin_exit] Too many arguments.\n");
             write(STDERR_FILENO, "minishell: exit: too many arguments\n", 36);
             set_global_exit_status(1);
             return (1);
        }
    }
    printf("[DEBUG builtin_exit] Final exit, code: %d\n", exit_code_val);
    gc_free_all();
    // rl_clear_history();
    exit(exit_code_val);
    return (0); // لن يصل الكود لهنا
}