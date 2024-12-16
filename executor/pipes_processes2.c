/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_processes2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:46:34 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/16 10:30:37 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	child_pros(t_cmdtable *cmd, t_var *vars, t_data *core, int *fd)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
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
		path_finder(vars, core, core->env, cmd->args);
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
		path_finder(vars, core, core->env, cmd->args);
	free_exit(core);
	exit(core->exit_status);
}

void	no_pipe_status(char **files, int status, t_data *core, pid_t second)
{
	setup_signal_handler(SIGINT, sig_int_parent2);
	waitpid(second, &status, 0);
	if (WIFEXITED(status))
		core->exit_status = WEXITSTATUS(status);
	here_doc_file_del(files);
}

void	no_pipe_child_proc(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	//setup_signal_handler(SIGQUIT, sig_quit_child);// I think here
	if (vars->del_files)
		simple_free(vars->del_files);
	if (cmd->next != NULL)
		cmd = multi_redirections(cmd, core, vars);
	execution_pro(cmd, core, vars, fd);
}

void	no_pipe_exe(t_cmdtable *cmd, t_data *core, t_var *vars, int status)
{
	int		fd[2];
	pid_t	second;
	char	**files;

	files = NULL;
	vars->file_error = 0;
	here_doc_creator(cmd, core, &files, 0);
	vars->del_files = files;
	if ((cmd->isbuiltin != 0 && cmd->isbuiltin != 1) || cmd->isprinted == 2)
		builtin_cmds(cmd, core);
	else
	{
		//setup_signal_handler(SIGQUIT, sig_quit_child); // bruh where should this go
		second = fork();
		if (second == -1)
			pipe_error(fd, core);
		if (second == 0)
			no_pipe_child_proc(cmd, core, vars, fd);
		else
			no_pipe_status(files, status, core, second);
	}
	free_cmdtable(&cmd);
	return ;
}
