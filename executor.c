/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:03:51 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/11 11:12:51 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	path_finder2(t_var *vars, char **envp, char *argv, int i)
{
	pid_t exe;
	int	status;
	int command_found = 0;
	
	while (envp[i] && !ft_strnstr(envp[i], "PATH", 4))
		i++;
	if (!envp[i])
		error_handler();
	vars->store = ft_split(envp[i] + 5, ':');
	if (!vars->store)
		error_handler();
	vars->cmd = ft_split(argv, ' ');
	if (!vars->cmd)
		error_handler_split(vars->store);
	i = -1;
	while (vars->store[++i])
	{
		vars->comm = ft_strjoin(vars->store[i], "/");
		if (vars->comm == NULL)
			error_handler();
		vars->full_comm = ft_strjoin(vars->comm, vars->cmd[0]);
		if (vars->full_comm == NULL)
			error_handler();
		free(vars->comm);
		
		//Creates a child process to execute the command
		exe = fork();
		if (exe < 0)
		{
			perror("fork");
			free(vars->full_comm);
			continue ;
		}
		else if (exe == 0)
		{
			if (execve(vars->full_comm, vars->cmd, envp) == -1)
			{
				free(vars->full_comm);
				exit(1);
			}
		}
		else
		{
			waitpid(exe, &status, 0);
			free(vars->full_comm);
			if (status != 0)
				continue ;
			command_found = 1;
			break ;
		}
	}
	if (!command_found) //To do: Put this command into the path_finder_error also change the name
		write(2, "Error: Command not found\n", 19);
	path_finder_error(vars->store, vars->cmd);
}

void	child_pros(t_var *vars, char *argv[], char **envp, int *fd)
{
	int	i;

	i = 0;
	close(fd[0]);
	vars->fdin = open(argv[1], O_RDONLY);
	if (vars->fdin == -1)
		error_handler_fd(fd[1]);
	if (dup2(vars->fdin, STDIN_FILENO) == -1)
	{
		close(vars->fdin);
		error_handler_fd(fd[1]);
	}
	if (dup2(fd[1], STDOUT_FILENO) == -1)
	{
		close(vars->fdin);
		error_handler_fd(fd[1]);
	}
	close(fd[1]);
	close(vars->fdin);
	path_finder2(vars, envp, argv[2], i);
}

void	parent_pros(t_var *vars, char *argv[], char **envp, int *fd)
{
	int	i;

	i = 0;
	close(fd[1]);
	vars->fdout = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (vars->fdout == -1)
		error_handler_fd(fd[0]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		close(vars->fdout);
		error_handler_fd(fd[0]);
	}
	if (dup2(vars->fdout, STDOUT_FILENO) == -1)
	{
		close(vars->fdout);
		error_handler_fd(fd[0]);
	}
	close(fd[0]);
	close(vars->fdout);
	path_finder2(vars, envp, argv[3], i);
}

void	checker(int *fd)
{
	if (pipe(fd) == -1)
	{
		perror("Pipe Failure");
		exit(1);
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
			{
				vars->fdin = open(argv[1], O_RDONLY);
				if (vars->fdin == -1)
					error_handler_fd(fd[i][1]); //Note that the errohandler will need a change
				if (dup2(vars->fdin, STDIN_FILENO) == -1)
				{
					close(vars->fdin);
					error_handler_fd(fd[i][1]);
				}
				if (dup2(fd[i][1], STDOUT_FILENO) == -1)
				{
					close(vars->fdin);
					error_handler_fd(fd[i][1]);
				}
				close(vars->fdin);
			}
			else if (i == cmds - 1)
			{ //Last pipes
				vars->fdout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
				if (vars->fdout == -1)
					error_handler_fd(fd[i - 1][0]);
				if (dup2(fd[i - 1][0], STDIN_FILENO) == -1)
				{
					close(vars->fdout);
					error_handler_fd(fd[i - 1][0]); // change the errorhandler
				}
				if (dup2(vars->fdout, STDOUT_FILENO) == -1)
				{
					close(vars->fdout);
					error_handler_fd(fd[i - 1][0]);
				}
				close(vars->fdout);
			}
			else
			{ //Middle pipes 
				if (dup2(fd[i - 1][0], STDIN_FILENO) == -1)
					error_handler_fd(fd[i - 1][0]); // change the errorhandler
				if (dup2(fd[i][1], STDOUT_FILENO) == -1)
					error_handler_fd(fd[i][1]);
			}
					
			j = 0;
			while (j < cmds - 1)
			{
				close(fd[j][0]);
				close(fd[j][1]);
				j++;
			}
			path_finder2(vars, envp, argv[i + 2], 0);
		}
		i++;
	}
	j = 0;
	while (j < cmds - 1)
	{
		close(fd[j][0]);
		close(fd[j][1]);
		j++;
	}
	
	j = 0;
	while (j < cmds)
	{
		waitpid(vars->childid, NULL, 0);
		j++;
	}
}

int	executor(int argc, char *argv[], t_data *core)
{
	
	int		fd[2];
	t_var	vars;

	if (argc == 1) //No Pipes just command
	{
		path_finder2(&vars, core->env, argv[0], 0);//
	}
	if (argc == 5) //Singe Pipe
	{
		checker(fd);
		vars.childid = fork();
		if (vars.childid == -1)
		{
			perror("Error While Forking");
			close(fd[0]);
			close(fd[1]);
			return (1);
		}
		if (vars.childid == 0)
			child_pros(&vars, argv, core->env, fd);
		else
			parent_pros(&vars, argv, core->env, fd);
	}
	else if (argc > 5) // Mutli Pipe
	{
		mutilpe_pipe(&vars, argc, argv, core->env);
	}
	return (0);
}
