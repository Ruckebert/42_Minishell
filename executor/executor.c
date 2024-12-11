/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:03:51 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/11 15:39:13 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t g_interrupt_received;

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
		exit_com(core);
	return ;
}

void	pipe_error(int *fd)
{
	perror("Error While Forking");
	close(fd[0]);
	close(fd[1]);
	exit(1);
}

void	here_doc_creator(t_cmdtable *cmd, t_data *core, char ***files, int i)
{
	t_cmdtable	*current_cmd;
	char		*temp;

	current_cmd = cmd;
	if (here_doc_counter(cmd) != 0)
	{
		*files = ft_calloc(here_doc_counter(cmd) + 1, sizeof(char *));
		while (cmd)
		{
			if (cmd->redir_type == 10 || cmd->redir_type == 30)
			{
				temp = here_doc_tempfile(cmd, core, 0);
				free(cmd->redir);
				cmd->redir = ft_strdup(temp);
				if (g_interrupt_received != 0)
				{
					(*files)[i] = ft_strdup(cmd->redir);
					here_doc_file_del(*files);
					return ;
				}
				cmd->redir_type = 1;
				(*files)[i] = ft_strdup(cmd->redir);
				free(temp);
				i++;
			}
			cmd = cmd->next;
		}
		cmd = current_cmd;
	}
}

int	pipe_checker(t_cmdtable *cmd)
{
	t_cmdtable	*tmp;
	int			i;

	i = 0;
	tmp = cmd;
	while (cmd)
	{
		if (cmd->has_pipe_after == 1)
			i++;
		cmd = cmd->next;
	}
	cmd = tmp;
	return (i);
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
		single_pipe_exe(cmd, core, &vars);
	else if (pipe_checker(cmd) > 1)
	{
		second = fork();
		if (second == -1)
			return (1);
		if (second == 0)
			multi_pipe(&vars, cmd, core, 0);
		else
		{
			waitpid(second, &status, 0);
			if (WIFEXITED(status))
				core->exit_status = WEXITSTATUS(status);
			free_cmdtable(&cmd);
		}
	}
	return (0);
}
