#include "../minishell.h"

void gc_free_pid_list(t_pid_list *list)
{
    t_pid_list *tmp;

    while (list)
    {
        tmp = list;
        list = list->next;
        gc_free_ptr(tmp);
    }
}

t_pid_list *add_pid_to_list(t_pid_list *list, pid_t pid)
{
    t_pid_list *new_node;
    t_pid_list *current;

    new_node = gc_malloc(sizeof(t_pid_list));
    if (!new_node) return (list);
    new_node->pid = pid;
    new_node->next = NULL;
    if (!list) return new_node;
    current = list;
    while (current->next)
        current = current->next;
    current->next = new_node;
    return list;
}

void wait_for_all_pids(t_pid_list *pid_list, pid_t last_pid_for_status)
{
    t_pid_list *current;
    int is_last;

    current = pid_list;
    while (current)
    {
        is_last = (current->pid == last_pid_for_status);
        handle_parent_wait(current->pid, is_last);
        current = current->next;
    }
}

pid_t fork_and_exec_pipe_segment(t_cmd *cmd, t_data *data, int in_fd, int out_pipe_fd[2])
{
    pid_t pid;

    pid = fork();
    if (pid == -1) { perror("fork"); exit(1); }
    if (pid == 0)
    {
        if (in_fd != STDIN_FILENO)
            apply_redirection(in_fd, STDIN_FILENO);
        if (out_pipe_fd[1] != STDOUT_FILENO)
        {
            close(out_pipe_fd[0]);
            apply_redirection(out_pipe_fd[1], STDOUT_FILENO);
        }
        execute_command_in_child(cmd, data);
    }
    return pid;
}

void execute_pipeline(t_data *data)
{
    t_cmd *current_cmd;
    int pipe_fds[2];
    int prev_pipe_read_fd;
    t_pid_list *pid_list_head;
    pid_t current_child_pid;

    prev_pipe_read_fd = STDIN_FILENO;
    current_cmd = data->com;
    pid_list_head = NULL;
    current_child_pid = 0;
    while (current_cmd)
    {
        pipe_fds[0] = -1;
        pipe_fds[1] = -1;
        if (current_cmd->next)
        {
            if (pipe(pipe_fds) == -1) { perror("pipe"); return; }
        }
        current_child_pid = fork_and_exec_pipe_segment(current_cmd, data, prev_pipe_read_fd, pipe_fds);
        if (current_child_pid == -1) { break; }
        pid_list_head = add_pid_to_list(pid_list_head, current_child_pid);
        if (prev_pipe_read_fd != STDIN_FILENO)
            close(prev_pipe_read_fd);
        if (current_cmd->next)
        {
            close(pipe_fds[1]);
            prev_pipe_read_fd = pipe_fds[0];
        }
        if (current_cmd->in_type == T_HEREDOC && current_cmd->in_file)
            unlink(current_cmd->in_file);
        current_cmd = current_cmd->next;
    }
    wait_for_all_pids(pid_list_head, current_child_pid);
    gc_free_pid_list(pid_list_head);
}