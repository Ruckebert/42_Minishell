/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:40:28 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/09 10:49:36 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	here_doc_counter(t_cmdtable *cmd)
{
	int	total;

	total = 0;
	while (cmd)
	{
		if (cmd->redir_type == 10 || cmd->redir_type == 30)
			total++;
		cmd = cmd->next;
	}
	return (total);
}

void	multi_pipe_end(int i, int *childids, t_data *core, char **files)
{
	int	j;
	int	status;

	status = 0;
	j = 0;
	while (j < i)
	{
		waitpid(childids[j], &status, 0);
		j++;
	}
	if (WIFEXITED(status))
		core->exit_status = WEXITSTATUS(status);
	here_doc_file_del(files);
	exit(core->exit_status);
}

void	multi_pipe_fd(int *fd, t_var *vars,
	t_cmdtable *current_cmd, t_data *core)
{
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
		close(fd[0]);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			error_handler_fd(fd[1], current_cmd);
		close(fd[1]);
	}
}

void	multi_pipe_process(int *fd, t_var *vars,
	t_cmdtable *current_cmd, t_data *core)
{
	if (vars->childid == -1)
		error_handler();
	if (vars->childid == 0)
	{
		multi_pipe_fd(fd, vars, current_cmd, core);
		if (current_cmd->isbuiltin > 1)
		{
			builtin_cmds(current_cmd, core);
			exit(1);
		}
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

void	multi_pipe(t_var *vars, t_cmdtable *cmd, t_data *core, int i)
{
	char		**files;
	int			fd[2];
	int			*childids;
	t_cmdtable	*current_cmd;

	childids = ft_calloc((cmd_count(cmd) + 1), sizeof(int *));
	vars->prev_fd = -1;
	current_cmd = cmd;
	files = NULL;
	i = 0;
	here_doc_creator(current_cmd, core, &files, 0);
	while (current_cmd)
	{
		if (current_cmd->next && pipe(fd) == -1)
			error_handler();
		if (current_cmd->redir_type != 0)
			current_cmd = multi_redirections(current_cmd, core, vars);
		vars->childid = fork();
		multi_pipe_process(fd, vars, current_cmd, core);
		childids[i++] = vars->childid;
		current_cmd = current_cmd->next;
	}
	if (vars->prev_fd != -1)
		close(vars->prev_fd);
	multi_pipe_end(i, childids, core, files);
}
