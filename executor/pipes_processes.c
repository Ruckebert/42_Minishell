/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:18:29 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/14 10:59:12 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


//NOTEEEE that you need to change this with the updated verison in the executor function
void	child_pros(t_cmdtable *cmd, t_var *vars, t_data *core, int *fd)
{
	close(fd[0]);
	if (cmd->redir_type != 0)
		redirctions(cmd, core, vars, fd);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_handler_fd(fd[1]);
	close(fd[1]);
	if (cmd->isbuiltin == 1)
		echo_cmd(cmd, core);
	else if (cmd->isbuiltin > 1)
		builtin_cmds(cmd, core);
	else
		path_finder(vars, core, core->env, cmd->args, 0);
	exit(0);
}

void	parent_pros(t_cmdtable *cmd, t_var *vars,  t_data *core, int *fd)
{
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_handler_fd(fd[0]);
	close(fd[0]);
	if (cmd->redir_type != 0)
		redirctions(cmd, core, vars, fd);
	if (cmd->isbuiltin == 1)
		echo_cmd(cmd, core);
	else if (cmd->isbuiltin > 1)
		builtin_cmds(cmd, core);
	else
		path_finder(vars, core, core->env, cmd->args, 0);
	exit(0);
}

void	pipe_error(int *fd)
{
	perror("Error While Forking");
	close(fd[0]);
	close(fd[1]);
}

void	no_pipe_exe(t_cmdtable *cmd, t_data *core, t_var *vars)
{
	int		fd[2];
	pid_t	second;
	int		status = 0;

	if (cmd->isbuiltin != 0 && cmd->isbuiltin != 1)
		builtin_cmds(cmd, core);
	else
	{
		second = fork();
		if (second == -1)
		{
			pipe_error(fd);
			return ;
		}
		if (second == 0)
		{
			if (cmd->redir_type != 0)
				redirctions(cmd, core, vars, fd);
			if (cmd->isbuiltin == 1)
				echo_cmd(cmd, core);
			else
				path_finder(vars, core, core->env, cmd->args, 0);
		}
		else
		{
			waitpid(second, &status, 0);
			if (WIFEXITED(status))
				core->exit_status = WEXITSTATUS(status);
		}
	}
	return ;
}

void	child_parent_execution(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	int	status = 0;

	if (pipe(fd) == -1)
	{
		perror("Pipe Failure");
		return ;
	}
	vars->childid = fork();
	if (vars->childid == -1)
	{
		pipe_error(fd);
		return ;
	}
	if (vars->childid == 0)
		child_pros(cmd, vars, core, fd);
	else
	{
		waitpid(vars->childid, &status, 0);
		if (WIFEXITED(status))
			core->exit_status = WEXITSTATUS(status);
		parent_pros(cmd->next, vars, core, fd);
	}
}
