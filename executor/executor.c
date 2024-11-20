/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:03:51 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/19 18:26:18 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//Note! that im not sure whether or not some of the error_handler from the original works with the new code
//Note! 2 -> Alot of variables and variable inputs are gonna have to get changed when the data struct from the parser is somewhat complete
//Note! Note to self dont wear the blue sweater anymore

void	path_finder(t_var *vars, t_data *core, char **envp, char **argv, int i)
{
	if (argv[0] == NULL)
		return ;
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
	write(2,argv[0],ft_strlen(argv[0]));
	write(2,": command not found\n",20);
	//ft_printf("%s: command not found\n", argv[0]);
	free_split(vars->store); 
	core->exit_status = 127;
	exit(core->exit_status);
}


void	absolute_path_finder(t_data *core, char **envp, char **argv)
{
	if (access(argv[0], R_OK) == 0)
		execve(argv[0], argv, envp);
	write(2,argv[0],ft_strlen(argv[0]));
	write(2,": command not found\n",20);
	core->exit_status = 127;
	exit(core->exit_status);
}

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

void	child_pros(t_cmdtable *cmd, t_var *vars, t_data *core, int *fd)
{
	close(fd[0]);
	if (cmd->redir_type == 10)
		here_doc(cmd, core, STDIN_FILENO);
	if (cmd->redir_type != 0 && cmd->redir_type != 10)
		redirctions(cmd, core, vars, fd);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_handler_fd(fd[1]);
	close(fd[1]);
	if (cmd->isbuiltin == 1)
		echo_cmd(cmd, core);
	else if (cmd->isbuiltin > 1)
		builtin_cmds(cmd, core);
	else if (cmd->args[0] && ft_strchr(cmd->args[0], '/'))
		absolute_path_finder(core, core->env, cmd->args);
	else
		path_finder(vars, core, core->env, cmd->args, 0);
	exit(0);
}

void	parent_pros(t_cmdtable *cmd, t_var *vars,  t_data *core, int *fd)
{
	close(fd[1]);
	if (cmd->redir_type == 10)
		here_doc(cmd, core, fd[0]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_handler_fd(fd[0]);
	close(fd[0]);
	if (cmd->redir_type != 0 && cmd->redir_type != 10)
		redirctions(cmd, core, vars, fd);
	if (cmd->isbuiltin == 1)
		echo_cmd(cmd, core);
	else if (cmd->isbuiltin > 1)
		builtin_cmds(cmd, core);
	else if (cmd->args[0] && ft_strchr(cmd->args[0], '/'))
		absolute_path_finder(core, core->env, cmd->args);
	else
		path_finder(vars, core, core->env, cmd->args, 0);
	exit(0);
}

void	pipe_error(int *fd)
{
	perror("Error While Forking");
	close(fd[0]);
	close(fd[1]);
}

void	no_pipe_exe(t_cmdtable *cmd, t_data *core, t_var *vars)
{
	int		fd[2];
	pid_t	second;
	int		status = 0;

	if (cmd->isbuiltin != 0 && cmd->isbuiltin != 1)
		builtin_cmds(cmd, core);
	else
	{
		second = fork();
		if (second == -1)
		{
			pipe_error(fd);
			return ;
		}
		if (second == 0)
		{
			if (cmd->redir_type != 0)
				redirctions(cmd, core, vars, fd);
			if (cmd->isbuiltin == 1)
				echo_cmd(cmd, core);
			else if (cmd->args[0] && ft_strchr(cmd->args[0], '/'))
				absolute_path_finder(core, core->env, cmd->args);
			else
				path_finder(vars, core, core->env, cmd->args, 0);
		}
		else
		{
			waitpid(second, &status, 0);
			if (WIFEXITED(status))
				core->exit_status = WEXITSTATUS(status);
		}
	}
	return ;
}

void	child_parent_execution(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	int	status = 0;

	if (pipe(fd) == -1)
	{
		perror("Pipe Failure");
		return ;
	}
	vars->childid = fork();
	if (vars->childid == -1)
	{
		pipe_error(fd);
		return ;
	}
	if (vars->childid == 0)
		child_pros(cmd, vars, core, fd);
	else
	{
		waitpid(vars->childid, &status, 0);
		if (WIFEXITED(status))
			core->exit_status = WEXITSTATUS(status);
		parent_pros(cmd->next, vars, core, fd);
	}
}

void	single_pipe_exe(t_cmdtable *cmd, t_data *core, t_var *vars)
{
	int		fd[2];
	pid_t 	second;
	int		status = 0;

	if (cmd->isbuiltin == 4)
		builtin_cmds(cmd, core);
	second = fork();
	if (second == -1)
	{
		pipe_error(fd);
		return ;
	}
	if (second == 0)
		child_parent_execution(cmd, core, vars, fd);
	else
	{
		waitpid(second, &status, 0);
		if (WIFEXITED(status))
			core->exit_status = WEXITSTATUS(status);
	}	
}

int	executor(t_cmdtable *cmd, t_data *core)
{
	int		fd[2];
	t_var	vars;
	pid_t 	second;
	int		status = 0;

	if (cmd->has_pipe_after != 1)
		no_pipe_exe(cmd, core, &vars);
	else if (cmd->has_pipe_after == 1 && cmd->next->has_pipe_after != 1) //Singe Pipe
		single_pipe_exe(cmd, core, &vars);
	else if (cmd->has_pipe_after == 1 && cmd->next->has_pipe_after == 1) // Mutli Pipe
	{
		second = fork();
		if (second == -1)
			return (pipe_error(fd), 1);
		if (second == 0)
			multi_pipe(&vars, cmd, core, core->env);
		else
		{
			waitpid(second, NULL, 0);
			if (WIFEXITED(status))
				core->exit_status = WEXITSTATUS(status);
		}
	}
	return (0);
}
