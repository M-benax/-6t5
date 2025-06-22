/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_core.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 18:46:55 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/17 18:00:17 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_builtin(char *cmd_name)
{
	if (!cmd_name) return (0);
	if (ft_strcmp(cmd_name, "echo") == 0 ||
		ft_strcmp(cmd_name, "cd") == 0 ||
		ft_strcmp(cmd_name, "pwd") == 0 ||
		ft_strcmp(cmd_name, "export") == 0 ||
		ft_strcmp(cmd_name, "unset") == 0 ||
		ft_strcmp(cmd_name, "env") == 0 ||
		ft_strcmp(cmd_name, "exit") == 0)
		return (1);
	return (0);
}

void execute_builtin_parent(t_cmd *cmd, t_data *data)
{
	int status_code = 0;
	
	if (!cmd || !cmd->args || !cmd->args[0]) {
		data->last_exit_status = EXIT_CMD_NOT_FOUND; 
		return;
	}
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(cmd->args, data);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		status_code = builtin_cd(cmd->args, &(data->tenv), data);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		status_code = builtin_pwd(data->tenv, data);
	else if (ft_strcmp(cmd->args[0], "echo") == 0)
		status_code = builtin_echo(cmd->args);
	else if (ft_strcmp(cmd->args[0], "env") == 0) {
		if (!cmd->args[1]) 
			print_env(data->tenv);
		else { 
			display_error_message("env", cmd->args[1], "No such file or directory"); 
			status_code = EXIT_CMD_CANNOT_EXECUTE; 
		}
	} else if (ft_strcmp(cmd->args[0], "export") == 0) {
		if (!cmd->args[1]) 
			print_export(data->tenv, data);
		else 
			status_code = handel_export(data);
	} else if (ft_strcmp(cmd->args[0], "unset") == 0)
		status_code = unset(cmd->args, &(data->tenv));
	else {
		display_error_message(cmd->args[0], NULL, "builtin not handled in parent");
		status_code = EXIT_GENERAL_ERROR;
	}
	if (ft_strcmp(cmd->args[0], "exit") != 0)
		data->last_exit_status = status_code;
}

int execute_builtin_child(t_cmd *cmd, t_data *data)
{
	if (!cmd || !cmd->args || !cmd->args[0]) 
		return (EXIT_CMD_NOT_FOUND);
	if (ft_strcmp(cmd->args[0], "echo") == 0) 
		return (builtin_echo(cmd->args));
	if (ft_strcmp(cmd->args[0], "pwd") == 0) 
		return (builtin_pwd(data->tenv, data));
	if (ft_strcmp(cmd->args[0], "env") == 0 && !cmd->args[1]) {
		print_env(data->tenv); 
		return (0);
	}
	if (ft_strcmp(cmd->args[0], "export") == 0 && !cmd->args[1]) {
		print_export(data->tenv, data); 
		return (0);
	}
	if (ft_strcmp(cmd->args[0], "exit") == 0) {
		int exit_s = data->last_exit_status;
		if (cmd->args[1] && is_str_numeric(cmd->args[1])) {
			exit_s = ft_atoi(cmd->args[1]) % 256;
			if (exit_s < 0) exit_s += 256;
		} else if (cmd->args[1]) {
			display_error_message("exit", cmd->args[1], "numeric argument required");
			exit_s = EXIT_SYNTAX_ERROR;
		}
		exit(exit_s);
	}
	if (ft_strcmp(cmd->args[0], "cd") == 0) 
		return builtin_cd(cmd->args, &(data->tenv), data);
	if (ft_strcmp(cmd->args[0], "export") == 0 && cmd->args[1]) 
		return handel_export(data);
	if (ft_strcmp(cmd->args[0], "unset") == 0) 
		return unset(cmd->args, &(data->tenv));

	return (EXIT_CMD_NOT_FOUND);
}