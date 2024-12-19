/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:40:28 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/19 11:09:39 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	multi_pipe_end(int i, int *childids, t_data *core, char **files)
{
	int	j;
	int	status;

	status = 0;
	j = 0;
	while (j <= i)
	{
		setup_signal_handler(SIGINT, sig_int_parent3);
		waitpid(childids[j], &status, 0);
		j++;
	}
	if (WIFEXITED(status))
		core->exit_status = WEXITSTATUS(status);
	here_doc_file_del(files);
	free(childids);
	free_exit(core);
	exit(core->exit_status);
}

void	multi_pipe_fd(int *fd, t_var *vars,
	t_cmdtable *current_cmd, t_data *core)
{
	if (vars->del_files)
		simple_free(vars->del_files);
	if (vars->prev_fd != -1)
	{
		if (dup2(vars->prev_fd, STDIN_FILENO) == -1)
			error_handler_fd(vars->prev_fd, current_cmd);
		close(vars->prev_fd);
		if (current_cmd->redir_type == 40)
			redirctions(current_cmd, core, vars, NULL);
	}
	if (current_cmd->next)
	{
		if (!(current_cmd->isbuiltin >= 1))
			close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			error_handler_fd(fd[1], current_cmd);
		if (!(current_cmd->isbuiltin >= 1))
			close(fd[1]);
	}
	if (!(current_cmd->isbuiltin > 1))
	{
		close(fd[0]);
		close(fd[1]);
	}
}

void	multi_pipe_process(int *fd, t_var *vars,
	t_cmdtable *current_cmd, t_data *core)
{
	if (vars->childid == -1)
		error_handler(core);
	if (vars->childid == 0)
	{
		multi_pipe_fd(fd, vars, current_cmd, core);
		if (current_cmd->isbuiltin > 1)
		{
			free(vars->childids);
			exit_pipe(fd, current_cmd);
			builtin_cmds(current_cmd, core);
			close(fd[0]);
			close(fd[1]);
			free_exit(core);
			exit(1);
		}
		free(vars->childids);
		execution_pro(current_cmd, core, vars, fd);
	}
	if (vars->prev_fd != -1)
		close(vars->prev_fd);
	if (current_cmd->next)
	{
		close(fd[1]);
		vars->prev_fd = fd[0];
	}
}

int	multi_pipe_loop(t_var *vars, t_cmdtable *current_cmd,
	t_data *core, int	*childids)
{
	int			i;
	int			fd[2];

	i = 0;
	while (current_cmd && current_cmd->isprinted != 2)
	{
		current_cmd->isprinted = 0;
		if (current_cmd->next && pipe(fd) == -1)
			error_handler(core);
		if (current_cmd->redir_type != 0)
			current_cmd = multi_redirections(current_cmd, core, vars);
		vars->childid = fork();
		vars->childids = childids;
		multi_pipe_process(fd, vars, current_cmd, core);
		childids[i++] = vars->childid;
		current_cmd = current_cmd->next;
	}
	return (i);
}

void	multi_pipe(t_var *vars, t_cmdtable *cmd, t_data *core, int i)
{
	char		**files;
	int			*childids;
	t_cmdtable	*current_cmd;

	childids = ft_calloc((cmd_count(cmd)), sizeof(int *));
	if (!childids)
		export_malloc_error(core, NULL);
	vars->prev_fd = -1;
	current_cmd = cmd;
	files = NULL;
	here_doc_creator(current_cmd, core, &files, 0);
	vars->del_files = files;
	i = multi_pipe_loop(vars, current_cmd, core, childids);
	if (vars->prev_fd != -1)
		close(vars->prev_fd);
	if (cmd->isprinted == 2)
		multi_pipe_end(i, childids, core, NULL);
	multi_pipe_end(i, childids, core, files);
}
