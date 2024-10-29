/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:03:51 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/29 11:18:23 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Note! that im not sure whether or not some of the error_handler from the original works with the new code
//Note! 2 -> Alot of variables and variable inputs are gonna have to get changed when the data struct from the parser is somewhat complete
//Note! Note to self dont wear the blue sweater anymore

void	here_doc(t_cmdtable *cmd, int *fd);

//Old Pathfinder works but exits the program
void	path_finder(t_var *vars, char **envp, char **argv, int i)
{
	while (envp[i] && !ft_strnstr(envp[i], "PATH", 4))
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
	ft_printf("%s: command not found\n", argv[0]);
	//path_finder_error(argv);
}
//OG Pathfinder
void	path_finder2(t_var *vars, char **envp, char **argv, int i)
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
			if (execve(vars->full_comm, argv, envp) == -1)
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
	//path_finder_error(argv);
}

//
//
// 		Pathfinders Above
//
//
//
//Piping the Child and leting the Parents wait ü

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
}

void	file_input(t_cmdtable *cmd, t_var *vars, int *fd);

void	child_pros(t_cmdtable *cmd, t_var *vars,  t_data *core, int *fd)
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
		if (cmd->isbuiltin == 1)
			builtin_cmds(cmd, core);
		else
			path_finder(vars, core->env, cmd->args, 0);
	}
}

void	file_output(t_cmdtable *cmd, t_var *vars, int *fd);

void	parent_pros(t_cmdtable *cmd, t_var *vars,  t_data *core, int *fd)
{
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_handler_fd(fd[0]);
	close(fd[0]);
	if (cmd->redir_type == 2)
		file_output(cmd, vars, fd);
	if (cmd->isbuiltin != 0)
		builtin_cmds(cmd, core);
	else
		path_finder(vars, core->env, cmd->args, 0);
}

void	file_input(t_cmdtable *cmd, t_var *vars, int *fd)
{
	vars->fdin = open(cmd->redir, O_RDONLY);
	if (vars->fdin == -1)
		error_handler_fd(fd[1]);
	if (cmd->args[0] != NULL) //Before Duplicating check if theirs a file 
	{
		if (dup2(vars->fdin, STDIN_FILENO) == -1)
		{
			close(vars->fdin);
			error_handler_fd(fd[1]);
		}
	}
	close(vars->fdin);
}

void	file_output(t_cmdtable *cmd, t_var *vars, int *fd)
{
	vars->fdout = open(cmd->redir, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (vars->fdout == -1)
		error_handler_fd(fd[0]);
	if (cmd->args[0] != NULL) //Before Duplicating check if theirs a file
	{
		if (dup2(vars->fdout, STDOUT_FILENO) == -1)
		{
			close(vars->fdout);
			error_handler_fd(fd[0]);
		}
	}
	close(vars->fdout);
}

void redirctions(t_cmdtable *cmd, t_var *vars, int *fd)
{
	if (cmd->redir_type == 1)
		file_input(cmd, vars, fd);
	else if (cmd->redir_type == 2)
		file_output(cmd, vars, fd);
}

//the Command Data Struct is only temporay
int	executor(t_cmdtable *cmd, t_data *core)
{
	int		fd[2];
	t_var	vars;
	pid_t second;

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
	
	if (cmd->has_pipe_after != 1) //No Pipes just command, only cmd does not take any params idk why
	{
		if (cmd->isbuiltin != 0 && cmd->isbuiltin != 1)
			builtin_cmds(cmd, core);
		else
		{
			second = fork();
			if (second == -1)
			{
				perror("Error While Forking");
				close(fd[0]);
				close(fd[1]);
				return (1);
			}
			if (second == 0)
			{
				if (cmd->redir_type != 0)
					redirctions(cmd, &vars, fd);
				if (cmd->isbuiltin == 1)
					echo_cmd(cmd, core);
				else
					path_finder(&vars, core->env, cmd->args, 0);
			}
			else
				waitpid(second, NULL, 0);
		}
	}
	else if (cmd->has_pipe_after == 1 && cmd->next->has_pipe_after != 1) //Singe Pipe
	{
		second = fork();
		if (second == -1)
		{
			perror("Error While Forking");
			close(fd[0]);
			close(fd[1]);
			return (1);
		}
		if (second == 0)
		{
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
				child_pros(cmd, &vars, core, fd);
			else
			{
				waitpid(vars.childid, NULL, 0);
				parent_pros(cmd->next, &vars, core, fd);
			}
		}
		else
			waitpid(second, NULL, 0);
	}
	else if (cmd->has_pipe_after == 1 && cmd->next->has_pipe_after == 1) // Mutli Pipe
	{
		second = fork();
		if (second == -1)
		{
			perror("Error While Forking");
			close(fd[0]);
			close(fd[1]);
			return (1);
		}
		if (second == 0)
			multi_pipe(&vars, cmd, core->env);
		else
			waitpid(second, NULL, 0);
	}
	return (0);
}
