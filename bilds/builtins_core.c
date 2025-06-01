#include "../minishell.h"

int is_builtin(char *cmd_name)
{
    // ... (الكود الموجود) ...
    if (cmd_name && (ft_strcmp(cmd_name, "echo") == 0 ||
        ft_strcmp(cmd_name, "cd") == 0 ||
        ft_strcmp(cmd_name, "pwd") == 0 ||
        ft_strcmp(cmd_name, "export") == 0 ||
        ft_strcmp(cmd_name, "unset") == 0 ||
        ft_strcmp(cmd_name, "env") == 0 ||
        ft_strcmp(cmd_name, "exit") == 0)) {
        printf("[DEBUG is_builtin] %s IS a builtin.\n", cmd_name);
        return (1);
    }
    printf("[DEBUG is_builtin] %s is NOT a builtin.\n", cmd_name ? cmd_name : "NULL");
    return (0);
}

void execute_builtin_parent(t_cmd *cmd, t_data *data)
{
    int status_code;

    status_code = 0;
    printf("[DEBUG exec_builtin_parent] Executing: %s\n", cmd->args[0]);
    // ... (باقي الشروط والاستدعاءات) ...
    // مثال لـ exit
    if (ft_strcmp(cmd->args[0], "exit") == 0)
        status_code = builtin_exit(cmd->args, data);
    // ...
    else if (ft_strcmp(cmd->args[0], "cd") == 0)
        status_code = builtin_cd(cmd->args, &(data->tenv));
    // ... (باقي الـ builtins) ...

    set_global_exit_status(status_code);
    printf("[DEBUG exec_builtin_parent] Finished %s, $? = %d\n", cmd->args[0], get_global_exit_status());
}

// ... (execute_builtin_child إذا كنت تستخدمها مع تتبع مشابه) ...

int execute_builtin_child(t_cmd *cmd, t_data *data)
{
    if (ft_strcmp(cmd->args[0], "echo") == 0)
        return (builtin_echo(cmd->args));
    else if (ft_strcmp(cmd->args[0], "pwd") == 0)
        return (builtin_pwd());
    else if (ft_strcmp(cmd->args[0], "env") == 0 && !cmd->args[1])
    {
        print_env(data->tenv);
        return (0);
    }
    else if (ft_strcmp(cmd->args[0], "export") == 0 && !cmd->args[1])
    {
        print_export(data->tenv);
        return (0);
    }
    return (127);
}

