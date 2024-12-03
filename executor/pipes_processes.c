/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:18:29 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/03 16:11:42 by aruckenb         ###   ########.fr       */
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
		echo_cmd(cmd, core);
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
		echo_cmd(cmd, core);
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

void	child_parent_execution(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
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
	{
		if (cmd->redir_type != 0)
			cmd = multi_redirections(cmd, core, vars);
		vars->file_error = 0;
		child_pros(cmd, vars, core, fd);
	}
	else
	{ 
		close(fd[1]);
		cmd = return_pipe(cmd);
		if (cmd->redir_type != 0)
		{
			cmd = multi_redirections(cmd, core, vars);
			vars->file_error = 0;
		}
		parent_pros(cmd, vars, core, fd);
	}
}

void	single_pipe_exe(t_cmdtable *cmd, t_data *core, t_var *vars)
{
	char **files = NULL;
	int		fd[2];
	pid_t 	second;
	int		i = 0;
	int		status = 0;
	t_cmdtable *current_cmd = cmd;
	
	if (here_doc_counter(cmd) != 0)
	{
		files = ft_calloc(here_doc_counter(cmd), sizeof(char *));
		while (cmd)
		{
			if (cmd->redir_type == 10 || cmd->redir_type == 30)
			{
				cmd->redir = ft_strdup(here_doc_tempfile(cmd, core, STDIN_FILENO));
				cmd->redir_type  = 1;
				files[i] = ft_strdup(cmd->redir);
				i++;
			}
			cmd = cmd->next;
		}
		cmd = current_cmd;
	}
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
		i = 0;
		if (files)
		{
			while (files[i])
			{
				unlink(files[i]);
				i++;
			}
		}
	}	
}
