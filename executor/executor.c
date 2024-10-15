/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:03:51 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/15 11:03:31 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//Note! that im not sure whether or not some of the error_handler from the original works with the new code
//Note! 2 -> Alot of variables and variable inputs are gonna have to get changed when the data struct from the parser is somewhat complete
//Note! Note to self dont wear the blue sweater anymore

void	here_doc(t_command *cmd, int *fd);

//Old Pathfinder works but exits the program
void	path_finder(t_var *vars, char **envp, char *argv, int i)
{
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
		execve(vars->full_comm, vars->cmd, envp);
		free(vars->full_comm);
	}
	path_finder_error(vars->store, vars->cmd);
}

// To Do: Find out why using this path_finder crashes it when using pipes
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
		write(2, "Error: Command not found\n", 26);
	path_finder_error(vars->store, vars->cmd);
}

//
//
//
// 		Pathfinders Above
//
//
//

//
//Piping the Child and leting the Parents wait ü
//

void	child_pros(t_command *cmd, t_var *vars, char *argv[], char **envp, int *fd)
{
	int	i;

	i = 0;
	close(fd[0]);
	if (cmd->here_doc)
		here_doc(cmd, fd);
	else
	{
		vars->fdin = open(argv[0], O_RDONLY);
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
		path_finder(vars, envp, argv[1], i);
	}
}

void	parent_pros(t_var *vars, char *argv[], char **envp, int *fd)
{
	int	i;

	i = 0;
	close(fd[1]);
	vars->fdout = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0777);
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
	path_finder(vars, envp, argv[2], i);
}

void	checker(int *fd)
{
	if (pipe(fd) == -1)
	{
		perror("Pipe Failure");
		exit(1);
	}
}

//
// All Mutiple Pipe
//

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
			path_finder(vars, envp, argv[i + 2], 0);
		}
		i++;
	}
	//closing_cmds(cmds, fd);
	j = -1;
	while (++j < cmds)
		waitpid(vars->childid, NULL, 0);
}


// File Input and Output and Here_Doc

void	here_doc(t_command *cmd, int *fd)
{
	char *line;
	int tmp_fd[2];
	
	if (pipe(tmp_fd) == -1)
		error_handler_fd(tmp_fd[1]);

	while (1)
	{
		line = get_next_line(tmp_fd[1]);
		if (!line || ft_strncmp(line, cmd->here_doc_delimiter, ft_strlen(line)) == 0)
			break ;
		write(tmp_fd[1], line, ft_strlen(line));
		free(line);
	}
	close(tmp_fd[1]);
	if (dup2(tmp_fd[0], STDIN_FILENO) == -1)
	{
		close(tmp_fd[0]);
		error_handler_fd(fd[1]);
	}
	close(tmp_fd[0]);	
}

void	file_input(t_command *cmd, t_var *vars, int *fd)
{
	vars->fdin = open(cmd->input_file, O_RDONLY);
		if (vars->fdin == -1)
			error_handler_fd(fd[1]);
		if (dup2(vars->fdin, STDIN_FILENO) == -1)
		{
			close(vars->fdin);
			error_handler_fd(fd[1]);
		}
		close(vars->fdin);
}

void	file_output(t_command *cmd, t_var *vars, int *fd)
{
	vars->fdout = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (vars->fdout == -1)
		error_handler_fd(fd[0]);
	if (dup2(vars->fdout, STDOUT_FILENO) == -1)
	{
		close(vars->fdout);
		error_handler_fd(fd[0]);
	}
	close(vars->fdout);
}

//the Command Data Struct is only temporay
int	executor(t_command *cmd, t_data *core)
{
	int		fd[2];
	t_var	vars;

	//To Do: Add a Here_Doc and Append
	if (cmd->here_doc)
	{
		here_doc(cmd, fd);
	}
	else if (cmd->input_file)
	{
		file_input(cmd, &vars, fd);
	}
	else if (cmd->output_file)
	{
		file_output(cmd, &vars, fd);
	}

	//To Do: Add or Find a way to include the inputs for the cmds
	if (cmd->arg_count == 1) //No Pipes just command, only cmd does not take any params idk why
	{
		path_finder2(&vars, core->env, cmd->args[0], 0);
	}
	else if (cmd->arg_count == 4) //Singe Pipe
	{
		//This Kinda Works but only with the old pathfinder using the new pathfinder causes a segfault
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
			child_pros(cmd, &vars, cmd->args, core->env, fd);
		else
			parent_pros(&vars, cmd->args, core->env, fd);
	}
	else if (cmd->arg_count > 5) // Mutli Pipe
		mutilpe_pipe(&vars, cmd->arg_count, cmd->args, core->env);
	return (0);
}
