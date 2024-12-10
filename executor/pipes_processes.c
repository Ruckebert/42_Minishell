/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:18:29 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/10 12:45:54 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmdtable	*return_pipe(t_cmdtable *cmd)
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
		free_exit(core);
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
		free_exit(core);
		exit(core->exit_status);
	}
}

void	child_parent_execution(t_cmdtable *cmd, t_data *core,
	t_var *vars, int *fd)
{
	int	status;

	status = 0;
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
	/*Freeing Part*/
	simple_free(core->env);
	if (core->export_env != NULL)
		simple_free(core->export_env);
	free(core->user);
	free(core->direct);
	free(core->line);
	/*Freeing Part*/
	exit(core->exit_status);
}

void	single_pipe_exe(t_cmdtable *cmd, t_data *core, t_var *vars)
{
	char	**files;
	int		fd[2];
	pid_t	second;
	int		status;

	status = 0;
	files = NULL;
	here_doc_creator(cmd, core, &files, 0);
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
