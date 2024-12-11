/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_processes2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:46:34 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/11 12:05:03 by marsenij         ###   ########.fr       */
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
	free_exit(core);
	exit(core->exit_status);
}

void	parent_pros(t_cmdtable *cmd, t_var *vars, t_data *core, int *fd)
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
	free_exit(core);
	exit(core->exit_status);
}

void	execution_pro(t_cmdtable *cmd, t_data *core, t_var *vars, int fd[2])
{
	if (cmd->redir_type != 0)
		redirctions(cmd, core, vars, fd);
	if (cmd->isbuiltin == 1)
		echo_cmd(cmd, core, 0);
	else if (cmd->args && ft_strchr(cmd->args[0], '/'))
		absolute_path_finder(core, core->env, cmd->args);
	else
		path_finder(vars, core, core->env, cmd->args, 0);
	exit(core->exit_status);
}

void	no_pipe_status(char **files, int status, t_data *core, pid_t second)
{
	setup_signal_handler(SIGINT, sig_handleINT_parent2);
	waitpid(second, &status, 0);
	if (WIFEXITED(status))
		core->exit_status = WEXITSTATUS(status);
	here_doc_file_del(files);
	free_cmdtable(&core->cmd);
}

void	no_pipe_exe(t_cmdtable *cmd, t_data *core, t_var *vars, int status)
{
	int		fd[2];
	pid_t	second;
	char	**files;

	files = NULL;
	here_doc_creator(cmd, core, &files, 0);
	if (cmd->isbuiltin != 0 && cmd->isbuiltin != 1)
		builtin_cmds(cmd, core);
	else
	{
		second = fork();
		if (second == -1)
			pipe_error(fd);
		if (second == 0)
		{
			if (cmd->next != NULL)
				cmd = multi_redirections(cmd, core, vars);
			execution_pro(cmd, core, vars, fd);
		}
		else
			no_pipe_status(files, status, core, second);
	}
	return ;
}
