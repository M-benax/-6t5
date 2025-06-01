#include "../minishell.h"

void start_execution(t_data *data)
{
    printf("[DEBUG start_execution] Entered.\n");
    if (!data || !data->com)
    {
        printf("[DEBUG start_execution] No data or command.\n");
        return;
    }
     if (!data->com->args || !data->com->args[0])
    {
        printf("[DEBUG start_execution] Command has no args[0].\n");
        // هذا يمكن أن يحدث إذا كان parse_command ينتج هيكل cmd فارغ لسبب ما
        // أو إذا كان الأمر فارغًا (مثل | |) وتم تمريره
        // يجب أن يتم التعامل مع الأوامر الفارغة في الـ parsing أو هنا
        set_global_exit_status(0); // لا يوجد أمر فعلي، ربما $? = 0
        return;
    }
    printf("[DEBUG start_execution] Command: %s\n", data->com->args[0]);

    if (!data->com->next && is_builtin(data->com->args[0]))
    {
        printf("[DEBUG start_execution] Identified as single builtin.\n");
        execute_builtin_parent(data->com, data);
    }
    else if (data->com->next)
    {
        printf("[DEBUG start_execution] Identified as pipeline.\n");
        execute_pipeline(data);
    }
    else
    {
        printf("[DEBUG start_execution] Identified as single external or child builtin.\n");
        execute_single_external_command(data->com, data);
    }
    printf("[DEBUG start_execution] Exiting. Current $? = %d\n", get_global_exit_status());
}

void execute_command_in_child(t_cmd *cmd, t_data *data)
{
    char *executable_path;
    char **envp_array;

    printf("[DEBUG child] PID %d: In execute_command_in_child for: %s\n", getpid(), cmd->args[0] ? cmd->args[0] : "NULL_CMD");

    handle_child_redirections(cmd); // افترض أنها تعمل بشكل صحيح

    if (cmd->args && cmd->args[0] && is_builtin(cmd->args[0]))
    {
        printf("[DEBUG child] PID %d: Executing builtin in child: %s\n", getpid(), cmd->args[0]);
        exit(execute_builtin_child(cmd, data));
    }

    printf("[DEBUG child] PID %d: Searching for executable: %s\n", getpid(), cmd->args[0]);
    executable_path = find_executable_path(cmd->args[0], data->tenv);
    if (!executable_path)
    {
        printf("[DEBUG child] PID %d: Executable NOT FOUND for: %s\n", getpid(), cmd->args[0]);
        write(STDERR_FILENO, "minishell: ", 11);
        write(STDERR_FILENO, cmd->args[0], ft_strlen(cmd->args[0]));
        write(STDERR_FILENO, ": command not found\n", 20);
        exit(127);
    }
    printf("[DEBUG child] PID %d: Found executable: %s for %s\n", getpid(), executable_path, cmd->args[0]);

    envp_array = convert_env_list_to_array(data->tenv);
    printf("[DEBUG child] PID %d: About to execve %s\n", getpid(), executable_path);
    execve(executable_path, cmd->args, envp_array);

    // إذا وصل الكود لهنا، يعني execve فشلت
    perror("minishell: execve error");
    printf("[DEBUG child] PID %d: execve FAILED for %s\n", getpid(), executable_path);
    free(executable_path); // إذا كان find_executable_path يستخدم malloc عادي لـ final_path
    gc_free_array(envp_array); // إذا كان envp_array وداخله مخصصين بـ gc_malloc
    exit(126);
}

void execute_single_external_command(t_cmd *cmd, t_data *data)
{
    pid_t pid;

    printf("[DEBUG exec_single] For command: %s\n", cmd->args[0] ? cmd->args[0] : "NULL_CMD");
    pid = fork();
    if (pid == -1)
    {
        perror("minishell: fork failed");
        set_global_exit_status(1);
        return;
    }
    if (pid == 0) // Child process
    {
        printf("[DEBUG exec_single] Child PID %d created for %s\n", getpid(), cmd->args[0]);
        execute_command_in_child(cmd, data);
    }
    else // Parent process
    {
        printf("[DEBUG exec_single] Parent PID %d waiting for child PID %d (%s)\n", getpid(), pid, cmd->args[0]);
        handle_parent_wait(pid, 1); // 1 يعني هذا آخر أمر (أو الوحيد)
        printf("[DEBUG exec_single] Parent PID %d finished waiting for child PID %d. $? = %d\n", getpid(), pid, get_global_exit_status());
        if (cmd->in_type == T_HEREDOC && cmd->in_file)
        {
            printf("[DEBUG exec_single] Unlinking heredoc: %s\n", cmd->in_file);
            unlink(cmd->in_file);
        }
    }
}