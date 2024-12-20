/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:03:51 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/20 12:46:08 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_interrupt_received;

void	builtin_cmds(t_cmdtable *cmd, t_data *core)
{
	if (cmd->isbuiltin == 2)
		cd_com(cmd, core);
	else if (cmd->isbuiltin == 3)
		pwd(core);
	else if (cmd->isbuiltin == 4)
		export(cmd, core);
	else if (cmd->isbuiltin == 5)
		unset(cmd, core);
	else if (cmd->isbuiltin == 6)
		env(core);
	else if (cmd->isbuiltin == 7)
		exit_com(core, cmd);
	return ;
}

void	pipe_error(int *fd, t_data *core)
{
	fd = 0;
	fd[0] = 2;
	ft_putstr_fd("Error: Forking Issue", fd[0]);
	free_exit(core);
	exit(1);
}

int	hdoc_dup_and_return(char *temp, t_cmdtable *cmd, char ***files, int i)
{
	free(cmd->redir);
	cmd->redir = ft_strdup2(temp);
	if (g_interrupt_received != 0)
	{
		(*files)[i] = ft_strdup2(cmd->redir);
		free(temp);
		cmd->isprinted = 2;
		here_doc_file_del(*files);
		return (1);
	}
	cmd->redir_type = 1;
	(*files)[i] = ft_strdup(cmd->redir);
	if (!((*files)[i]))
	{
		free(temp);
		unlink(cmd->redir);
		export_malloc_error(address_getter(NULL), (*files));
		exit(1);
	}
	return (0);
}

void	here_doc_creator(t_cmdtable *cmd, t_data *core, char ***files, int i)
{
	t_cmdtable	*current_cmd;
	char		*temp;

	current_cmd = cmd;
	if (here_doc_counter(cmd) != 0)
	{
		*files = ft_calloc(here_doc_counter(cmd) + 1, sizeof(char *));
		if (!*files)
			export_malloc_error(core, NULL);
		while (cmd)
		{
			if (cmd->redir_type == 10 || cmd->redir_type == 30)
			{
				temp = here_doc_tempfile(cmd, core, 0);
				if (hdoc_dup_and_return(temp, cmd, files, i) == 1)
					return ;
				free(temp);
				i++;
			}
			cmd = cmd->next;
		}
		cmd = current_cmd;
	}
}

int	executor(t_cmdtable *cmd, t_data *core)
{
	t_var	vars;
	pid_t	second;
	int		status;

	status = 0;
	if (pipe_checker(cmd) == 0)
		no_pipe_exe(cmd, core, &vars, status);
	else if (pipe_checker(cmd) == 1)
		single_pipe_exe(cmd, core, &vars, status);
	else if (pipe_checker(cmd) > 1)
	{
		setup_signal_handler(SIGQUIT, sig_quit_child);
		second = fork();
		if (second == -1)
			pipe_error(0, core);
		if (second == 0)
			multi_pipe(&vars, cmd, core, 0);
		else
			pipe_wait(status, second, cmd, core);
	}
	return (0);
}
