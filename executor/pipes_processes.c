/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:18:29 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/17 12:38:10 by aruckenb         ###   ########.fr       */
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
		pipe_error(fd, core);
	if (vars->childid == 0)
	{
		cmd->isprinted = 0;
		if (cmd->redir_type != 0)
		{
			close(fd[0]);
			//if (close(fd[0]) == -1)
			//	close_fail(core, fd);
			close(fd[1]);
			//if (close(fd[1]) == -1)
			//	close_fail(core, fd);
			cmd = multi_redirections(cmd, core, vars);
		}
		vars->file_error = 0;
		child_pros(cmd, vars, core, fd);
		free_exit(core);
		exit(core->exit_status);
	}
}

void	parent_pipe(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	if (vars->childid2 == -1)
		pipe_error(fd, core);
	if (vars->childid2 == 0)
	{
		cmd->isprinted = 0;
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
	if (vars->del_files)
		simple_free(vars->del_files);
	if (pipe(fd) == -1)
		pipe_error(fd, core);
	vars->childid = fork();
	child_pipe(cmd, core, vars, fd);
	vars->childid2 = fork();
	parent_pipe(cmd, core, vars, fd);
	close(fd[0]);
	close(fd[1]);
	setup_signal_handler(SIGINT, sig_int_parent3);
	waitpid(vars->childid, &status, 0);
	waitpid(vars->childid2, &status, 0);
	if (WIFEXITED(status))
		core->exit_status = WEXITSTATUS(status);
	free_exit(core);
	exit(core->exit_status);
}

void	single_pipe_exe(t_cmdtable *cmd, t_data *core, t_var *vars, int status)
{
	char	**files;
	int		fd[2];
	pid_t	second;

	files = NULL;
	here_doc_creator(cmd, core, &files, 0);
	vars->del_files = files;
	if (cmd->isprinted == 2)
	{
		free_cmdtable(&cmd);
		return ;
	}
	setup_signal_handler(SIGQUIT, sig_quit_child);
	second = fork();
	if (second == -1)
		pipe_error(fd, core);
	if (second == 0)
		child_parent_execution(cmd, core, vars, fd);
	else
	{
		pipe_wait(status, second, cmd, core);
		here_doc_file_del(files);
	}
}
