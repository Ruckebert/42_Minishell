/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:40:28 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/28 13:04:36 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//To Do: Fix Mutilpe Pipes its bad XD
//Okay Funny thing is that it does create new files my guess something happens with the mutilpe file inputs which causes the program to run endless but also its the input.
void	first_pipe(t_var *vars, t_cmdtable *cmd, int fd)
{
	if (cmd->redir_type == 4)
	{
		//here_doc(cmd, fd);
	}
	else
	{	
		if (cmd->redir_type == 1)
			file_input(cmd, vars, &fd);
		if (dup2(fd, STDOUT_FILENO) == -1)
			error_handler_fd(fd);
		close(fd);
	}
}


void	last_pipe(t_var *vars, t_cmdtable *cmd, int fd)
{
	if (dup2(fd, STDIN_FILENO) == -1)
		error_handler_fd(fd);
	close(fd);
	if (cmd->redir_type == 2)
		file_output(cmd, vars, &fd);
}

void	closing_cmds(int cmds, int **fd)
{
	int j = 0;
	while (j < cmds - 1)
	{
		close(fd[j][0]);
		close(fd[j][1]);
		j++;
	}
}
void	multi_pipe(t_var *vars, t_cmdtable *cmd, char **envp)
{
	int		cmds = 0;
	t_cmdtable *tmp = cmd;
	int		i = 0;
	int		j = 0;
	
	while (tmp)
	{
		cmds++;
		tmp = tmp->next;
	}

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

	// Fork each command in the pipeline
	while (current_cmd)
	{
		vars->childid = fork();
		if (vars->childid == -1)
			error_handler();

		if (vars->childid == 0)
		{
			if (i == 0)
				first_pipe(vars, cmd, fd[i][1]);
			else if (i == cmds - 1)
				last_pipe(vars, cmd, fd[i - 1][0]);
			else
			{
				if (dup2(fd[i - 1][0], STDIN_FILENO) == -1)
					error_handler_fd(fd[i - 1][0]);
				if (cmd->redir_type == 1)
					file_output(cmd, vars, &fd[i - 1][0]);
				if (dup2(fd[i][1], STDOUT_FILENO) == -1)
					error_handler_fd(fd[i][1]);
				if (cmd->redir_type == 2)
					file_input(cmd, vars, &fd[i][1]);
			}


			j = 0;
			while (j < cmds - 1)
			{
				close(fd[j][0]);
				close(fd[j][1]);
				j++;
			}
			path_finder(vars, envp, current_cmd->args, 0);
			exit(0); 
		}
		current_cmd = current_cmd->next;
		i++;
	}

	j = 0;
	while (j < cmds - 1)
	{
		close(fd[j][0]);
		close(fd[j][1]);
		j++;
	}

	j = -1;
	while (++j < cmds)
		waitpid(vars->childid, NULL, 0);
}