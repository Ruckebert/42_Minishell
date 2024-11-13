/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:40:28 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/13 12:04:32 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	closing_cmds(int cmds, int fd[cmds - 1][2])
{
	int j = 0;
	while (j < cmds - 1)
	{
		close(fd[j][0]);
		close(fd[j][1]);
		j++;
	}
}

int	cmd_count(t_cmdtable *cmd)
{
	int i;
	int total;

	i = 0;
	total = 0;
	while (cmd)
	{
		i = 0;
		while (cmd->args[i])
		{
			i++;
			total++;
			break ;
		}
		cmd = cmd->next;
	}
	//printf("Total:%d\n", total);
	return (total);
}

void	multi_pipe(t_var *vars, t_cmdtable *cmd, t_data *core, char **envp)
{
	int		cmds = cmd_count(cmd);
	int		status = 0;
	int		i = 0;
	int		j = 0;

	int fd[cmds - 1][2];

	while (i < cmds - 1)
	{
		if (pipe(fd[i]) == -1)
		{
			while (j < i)
			{
				close(fd[j][0]);
				close(fd[j][1]);
				j++;
			}
			error_handler();
		}	
		i++;
	}

	i = 0;
	t_cmdtable *current_cmd = cmd;

	while (current_cmd)
	{
		vars->childid = fork();
		if (vars->childid == -1)
			error_handler();
		if (vars->childid == 0)
		{
			if (i == 0)
			{
				if (dup2(fd[i][1], STDOUT_FILENO) == -1)
					error_handler_fd(fd[i][1]);
			}
			else if (i == cmds - 1)
			{
				if (dup2(fd[i - 1][0], STDIN_FILENO) == -1)
					error_handler_fd(fd[i - 1][0]);
			}
			else
			{
				if (dup2(fd[i - 1][0], STDIN_FILENO) == -1)
					error_handler_fd(fd[i - 1][0]);
				if (dup2(fd[i][1], STDOUT_FILENO) == -1)
					error_handler_fd(fd[i][1]);
			}
			if (current_cmd->redir != 0)
				redirctions(cmd, core, vars, *fd);
			
			closing_cmds(cmds, fd);
			if (current_cmd->isbuiltin == 1)
				echo_cmd(current_cmd, core);
			else
				path_finder(vars, core, envp, current_cmd->args, 0);
			exit(0);
		}
		current_cmd = current_cmd->next;
		i++;
	}
	closing_cmds(cmds, fd);
	j = -1;
	while (++j < cmds)
		waitpid(vars->childid, &status, 0);
	exit(core->exit_status);
}
