/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 15:34:33 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/27 15:34:02 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	file_input(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	if (access(cmd->redir, R_OK) != 0)
	{
		ft_putstr_fd(cmd->redir, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		core->exit_status = 1;
		exit(core->exit_status);
	}
	vars->fdin = open(cmd->redir, O_RDONLY);
	if (vars->fdin == -1)
	{
		error_handler_fd(fd[1], cmd);
		core->exit_status = 1;
		exit(core->exit_status);
	}
	if (cmd->args != NULL)
	{
		if (dup2(vars->fdin, STDIN_FILENO) == -1)
		{
			close(vars->fdin);
			error_handler_fd(fd[1], cmd);
			core->exit_status = 1;
			exit (core->exit_status);
		}
	}
	close(vars->fdin);
}

void	file_output(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	vars->fdout = open(cmd->redir, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (vars->fdout == -1)
	{
		error_handler_fd(fd[0], cmd);
		core->exit_status = 1;
		exit (core->exit_status);
	}
	if (cmd->args[0] != NULL && vars->file_error != 1)
	{
		if (dup2(vars->fdout, STDOUT_FILENO) == -1)
		{
			close(vars->fdout);
			error_handler_fd(fd[0], cmd);
			core->exit_status = 1;
			exit(core->exit_status);
		}
	}
	close(vars->fdout);;
}

void	file_append(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	vars->fdout = open(cmd->redir, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (vars->fdout == -1)
	{
		error_handler_fd(fd[0], cmd);
		core->exit_status = 1;
		exit (core->exit_status);
	}
	if (cmd->args[0] != NULL)
	{
		if (dup2(vars->fdout, STDOUT_FILENO) == -1)
		{
			close(vars->fdout);
			error_handler_fd(fd[0], cmd);
			core->exit_status = 1;
			exit (core->exit_status);
		}
	}
	close(vars->fdout);
}

void	redirctions(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	if (cmd->redir_type == 1)
		file_input(cmd, core, vars, fd);
	else if (cmd->redir_type == 2)
		file_output(cmd, core, vars, fd);
	else if (cmd->redir_type == 10 || cmd->redir_type == 30)
		here_doc(cmd, core, STDIN_FILENO);
	else if (cmd->redir_type == 20)
		file_append(cmd, core, vars, fd);
	else if (cmd->redir_type == 40)
	{
		file_input(cmd, core, vars, fd);
		file_output(cmd->next, core, vars, fd);
	}
	return ;
}
