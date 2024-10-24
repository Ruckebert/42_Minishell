/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:40:28 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/24 12:41:08 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//To Do: Fix Mutilpe Pipes its bad XD
//Okay Funny thing is that it does create new files my guess something happens with the mutilpe file inputs which causes the program to run endless but also its the input.
void	first_pipe(t_var *vars, char *argv[], int fd)
{
	vars->fdin = open(argv[1], O_RDONLY);
	if (vars->fdin == -1)
		error_handler_fd(fd); //Note that the errohandler will need a change
	if (dup2(vars->fdin, STDIN_FILENO) == -1)
	{
		close(vars->fdin);
		error_handler_fd(fd);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(vars->fdin);
		error_handler_fd(fd);
	}
	close(vars->fdin);
}

void	last_pipe(t_var *vars, int argc, char *argv[], int fd)
{
	vars->fdout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (vars->fdout == -1)
		error_handler_fd(fd);
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(vars->fdout);
		error_handler_fd(fd); // change the errorhandler
	}
	if (dup2(vars->fdout, STDOUT_FILENO) == -1)
	{
		close(vars->fdout);
		error_handler_fd(fd);
	}
	close(vars->fdout);
	
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
void	mutilpe_pipe(t_var *vars, int argc, char *argv[], char **envp)
{
	int cmds = argc - 3;
	int fd[cmds - 1][2];
	int i = 0;
	int j = 0;

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
	while (i < cmds)
	{
		vars->childid = fork();
		if (vars->childid == -1)
			error_handler();
		if (vars->childid == 0)
		{
			if (i == 0)
				first_pipe(vars, argv, fd[i][1]);
			else if (i == cmds - 1)
				last_pipe(vars, argc, argv, fd[i - 1][0]);
			else
			{
				if (dup2(fd[i - 1][0], STDIN_FILENO) == -1)
					error_handler_fd(fd[i - 1][0]);
				if (dup2(fd[i][1], STDOUT_FILENO) == -1)
					error_handler_fd(fd[i][1]);
			}

			//For some reason its not accepting the format of the closing_cmds
			//To Do: Test and Fix all of it honestly XD
			//closing_cmds(cmds, fd);
			j = 0;
			while (j < cmds - 1)
			{
				close(fd[j][0]);
				close(fd[j][1]);
				j++;
			}
			path_finder(vars, envp, argv, 0);
		}
		i++;
	}
	//closing_cmds(cmds, fd);
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