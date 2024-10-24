/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:03:51 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/24 13:46:44 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

//Note! that im not sure whether or not some of the error_handler from the original works with the new code
//Note! 2 -> Alot of variables and variable inputs are gonna have to get changed when the data struct from the parser is somewhat complete
//Note! Note to self dont wear the blue sweater anymore

void	here_doc(t_cmdtable *cmd, int *fd);

//Old Pathfinder works but exits the program
void	path_finder(t_var *vars, char **envp, char **argv, int i)
{
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		error_handler();
	vars->store = ft_split(envp[i] + 5, ':');
	if (!vars->store)
		error_handler();
	i = -1;
	while (vars->store[++i])
	{
		vars->comm = ft_strjoin(vars->store[i], "/");
		if (vars->comm == NULL)
			error_handler();
		vars->full_comm = ft_strjoin(vars->comm, argv[0]);
		if (vars->full_comm == NULL)
			error_handler();
		free(vars->comm);
		execve(vars->full_comm, argv, envp);
		free(vars->full_comm);
	}
	path_finder_error(vars->store, argv);
}

void	path_finder2(t_var *vars, char **envp, char **argv, int i)
{
	pid_t pid;

	i = 0;
    while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
        i++;
    if (!envp[i])
        error_handler();

    vars->store = ft_split(envp[i] + 5, ':');
    if (!vars->store)
        error_handler();

    i = 0;
    while (vars->store[i])
    {
        vars->comm = ft_strjoin(vars->store[i], "/");
        if (vars->comm == NULL)
            error_handler();
	
        vars->full_comm = ft_strjoin(vars->comm, argv[0]);
        if (vars->full_comm == NULL)
            error_handler();

        // Fork a new process
		if (access(vars->full_comm, X_OK) == 0)
		{
        	pid = fork();
        	if (pid == -1)
            	error_handler();
        	else if (pid == 0)
				execve(vars->full_comm, argv, envp);
		}
        free(vars->comm);
        free(vars->full_comm);
		i++;
    }
}
// To Do: Find out why using this path_finder crashes it when using pipes
/*void	path_finder2(t_var *vars, char **envp, char **argv, int i)
{
	pid_t exe;
	int	status;
	int command_found = 0;
	
	i = 0;
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		error_handler();
	vars->store = ft_split(envp[i] + 5, ':');
	if (!vars->store)
		error_handler();
	i = -1;
	while (vars->store[++i])
	{
		vars->comm = ft_strjoin(vars->store[i], "/");
		if (vars->comm == NULL)
			error_handler();
		//Instead of cmd it will be the first argv 
		vars->full_comm = ft_strjoin(vars->comm, argv[0]);
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
			if (access(vars->full_comm, X_OK) == 0)
				execve(vars->full_comm, argv, envp);
			else
				exit (1);
		}
		else
		{
			waitpid(exe, &status, 0);
			free(vars->full_comm);
			if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
			{
				command_found = 1;
				break ;
			}
		}
	}
	if (!command_found) //To do: Put this command into the path_finder_error also change the name
		write(2, "Error: Command not found\n", 26);
	path_finder_error(vars->store, vars->cmd);
}*/

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

void	file_input(t_cmdtable *cmd, t_var *vars, int *fd);

void	child_pros(t_cmdtable *cmd, t_var *vars, char **envp, int *fd)
{
	close(fd[0]);
	if (cmd->redir_type == 4)
		here_doc(cmd, fd);
	else
	{	
		if (cmd->redir_type == 1)
			file_input(cmd, vars, fd);
		if (dup2(fd[1], STDOUT_FILENO) == -1)
			error_handler_fd(fd[1]);
		close(fd[1]);
		path_finder(vars, envp, cmd->args, 0);
	}
}

void	file_output(t_cmdtable *cmd, t_var *vars, int *fd);

void	parent_pros(t_cmdtable *cmd, t_var *vars, char **envp, int *fd)
{
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_handler_fd(fd[0]);
	close(fd[0]);
	if (cmd->redir_type == 2)
		file_output(cmd, vars, fd);
	path_finder2(vars, envp, cmd->args, 0);
}

void	file_input(t_cmdtable *cmd, t_var *vars, int *fd)
{
	write(1, "3a", 2);
	vars->fdin = open(cmd->next->redir, O_RDONLY);
	if (vars->fdin == -1)
		error_handler_fd(fd[1]);
	if (dup2(vars->fdin, STDIN_FILENO) == -1)
	{
		close(vars->fdin);
		error_handler_fd(fd[1]);
	}
	close(vars->fdin);
}

void	file_output(t_cmdtable *cmd, t_var *vars, int *fd)
{
	vars->fdout = open(cmd->next->redir, O_WRONLY | O_CREAT | O_TRUNC, 0777);
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
int	executor(t_cmdtable *cmd, t_data *core)
{
	int		fd[2];
	t_var	vars;

	//To Do: Add a Here_Doc and Append
	/*if (cmd->redir_type == 3) //THis would be the here_doc
	{
		here_doc(cmd, fd);
	}*/
	
	int i = 0;
	while (cmd->args[i])
	{
		i++;
	}

	//To Do: Add or Find a way to include the inputs for the cmds
	if (cmd->has_pipe_after != 1) //No Pipes just command, only cmd does not take any params idk why
	{
		path_finder2(&vars, core->env, cmd->args, 0);
	}
	else if (cmd->has_pipe_after == 1 && cmd->next->has_pipe_after != 1) //Singe Pipe
	{
		//This Kinda Works but only with the old pathfinder using the new pathfinder causes a segfault
		if (pipe(fd) == -1)
		{
			perror("Pipe Failure");
			exit(1);
		}
		vars.childid = fork();
		if (vars.childid == -1)
		{
			perror("Error While Forking");
			close(fd[0]);
			close(fd[1]);
			return (1);
		}
		if (vars.childid == 0)
			child_pros(cmd, &vars, core->env, fd);
		else
		{
			waitpid(vars.childid, NULL, 0);
			parent_pros(cmd->next, &vars, core->env, fd);
		}
	}
	else if (i > 5) // Mutli Pipe
		mutilpe_pipe(&vars, i, cmd->args, core->env);
	return (0);
}
