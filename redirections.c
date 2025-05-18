#include "minishell.h"

void apply_redirections(t_cmd *cmd)
{
	int fd;

	if (cmd->in_type == T_REDIR_IN && cmd->in_file)
	{
		fd = open(cmd->in_file, O_RDONLY);
		if (fd == -1)
			perror("open input");
		else
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}
	else if (cmd->in_type == T_HEREDOC && cmd->in_file)
	{
		fd = open(cmd->in_file, O_RDONLY);
		if (fd == -1)
			perror("open heredoc file");
		else
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
	}

	if (cmd->out_type == T_REDIR_OUT && cmd->out_file)
	{
		fd = open(cmd->out_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd == -1)
			perror("open output");
		else
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
	else if (cmd->out_type == T_APPEND && cmd->out_file)
	{
		fd = open(cmd->out_file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd == -1)
			perror("open append");
		else
		{
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
	}
}
