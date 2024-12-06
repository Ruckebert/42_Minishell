/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:18:29 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/06 12:49:01 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_pros(t_cmdtable *cmd, t_var *vars, t_data *core, int *fd)
{
	close(fd[0]);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_handler_fd(fd[1], cmd);
	close(fd[1]);
	if (cmd->redir_type != 0 && cmd->redir_type != 10 && cmd->redir_type != 30)
		redirctions(cmd, core, vars, fd);
	if (cmd->isbuiltin == 1)
		echo_cmd(cmd, core, 0);
	else if (cmd->isbuiltin > 1)
		builtin_cmds(cmd, core);
	else if (cmd->args && ft_strchr(cmd->args[0], '/'))
		absolute_path_finder(core, core->env, cmd->args);
	else
		path_finder(vars, core, core->env, cmd->args, 0);
	exit(0);
}

void	parent_pros(t_cmdtable *cmd, t_var *vars,  t_data *core, int *fd)
{
	close(fd[1]);
	if (cmd->redir_type == 2 || cmd->redir_type == 20)
		redirctions(cmd, core, vars, fd);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_handler_fd(fd[0], cmd);
	close(fd[0]);
	if (cmd->redir_type == 1 || cmd->redir_type == 40)
		redirctions(cmd, core, vars, fd);
	if (cmd->isbuiltin == 1)
		echo_cmd(cmd, core, 0);
	else if (cmd->isbuiltin > 1)
		builtin_cmds(cmd, core);
	else if (cmd->args && ft_strchr(cmd->args[0], '/'))
		absolute_path_finder(core, core->env, cmd->args);
	else
		path_finder(vars, core, core->env, cmd->args, 0);
	exit(core->exit_status);
}

t_cmdtable *return_pipe(t_cmdtable *cmd)
{
	if (cmd->has_pipe_after == 1)
		return (cmd->next);
	while (cmd)
	{
		if (cmd->has_pipe_after == 1)
			break ;
		cmd = cmd->next;
	}
	return (cmd->next);
}


void	child_pipe(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	if (vars->childid == -1)
	{
		pipe_error(fd);
		return ;
	}
	if (vars->childid == 0)
	{
		if (cmd->redir_type != 0)
			cmd = multi_redirections(cmd, core, vars);
		vars->file_error = 0;
		child_pros(cmd, vars, core, fd);
		exit(core->exit_status);
	}
	
}

void	parent_pipe(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	if (vars->childid2 == -1)
	{
		pipe_error(fd);
		return ;
	}
	if (vars->childid2 == 0)
	{
		cmd = return_pipe(cmd);
		if (cmd->redir_type != 0)
			cmd = multi_redirections(cmd, core, vars);
		vars->file_error = 0;
		parent_pros(cmd, vars, core, fd);
		exit(core->exit_status);
	}
}

void	child_parent_execution(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	int		status = 0;
	
	if (pipe(fd) == -1)
		pipe_error(fd);

	vars->childid = fork();
	child_pipe(cmd, core, vars, fd);
	vars->childid2 = fork();
	parent_pipe(cmd, core, vars, fd);
	close(fd[0]);
    close(fd[1]);
	waitpid(vars->childid, &status, 0);
    waitpid(vars->childid2, &status, 0);
    if (WIFEXITED(status))
		core->exit_status = WEXITSTATUS(status);
	exit(core->exit_status);
}

void	single_pipe_exe(t_cmdtable *cmd, t_data *core, t_var *vars)
{
	char **files = NULL;
	int		fd[2];
	pid_t 	second;
	int		status = 0;
	
	here_doc_creator(cmd, core, files);
	second = fork();
	if (second == -1)
	{
		pipe_error(fd);
		return ;
	}
	if (second == 0)
		child_parent_execution(cmd, core, vars, fd);
	else
	{
		waitpid(second, &status, 0);
		if (WIFEXITED(status))
			core->exit_status = WEXITSTATUS(status);
		here_doc_file_del(files);
	}	
}
