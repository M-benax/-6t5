#include "../minishell.h"

int open_output_file(char *filename, int type)
{
    int flags;
    int fd;

    if (type == T_REDIR_OUT)
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    else
        flags = O_WRONLY | O_CREAT | O_APPEND;
    fd = open(filename, flags, 0644);
    if (fd == -1)
    {
        perror(filename);
        exit(1);
    }
    return (fd);
}

int open_input_file(char *filename)
{
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror(filename);
        exit(1);
    }
    return (fd);
}

void apply_redirection(int old_fd, int new_fd)
{
    if (dup2(old_fd, new_fd) == -1)
    {
        perror("dup2");
        exit(1);
    }
    close(old_fd);
}

void handle_child_redirections(t_cmd *cmd)
{
    int fd;

    if (cmd->in_file)
    {
        fd = open_input_file(cmd->in_file);
        apply_redirection(fd, STDIN_FILENO);
    }
    if (cmd->out_file)
    {
        fd = open_output_file(cmd->out_file, cmd->out_type);
        apply_redirection(fd, STDOUT_FILENO);
    }
}