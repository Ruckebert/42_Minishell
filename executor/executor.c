/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:03:51 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/03 15:00:22 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


//TO DO: Add a function to check the ending of the path function;
void	path_finder(t_var *vars, t_data *core, char **envp, char **argv, int i)
{
	int store;

	store = 0;
	if (argv == NULL)
		return ;
	while (envp[i] && !ft_strnstr(envp[i], "PATH", 4))
		i++;
	if (!envp[i])
	{
		//in this case the path does not exist
		if ((access(argv[0], R_OK) == 0))
			execve(argv[0], argv, envp);
		write(2,argv[0],ft_strlen(argv[0]));
		write(2, ": No such file or directory\n", 29);
		exit(1);
	}
	vars->store = ft_split(envp[i] + 5, ':');
	if (!vars->store)
		error_handler();
	store = i;
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
	if (ft_strlen(envp[store]) <= 5)
	{
		if ((access(argv[0], R_OK) == 0))
			execve(argv[0], argv, envp);
		write(2,argv[0],ft_strlen(argv[0]));
		write(2, ": No such file or directory\n", 29);
	}
	else
	{
		write(2,argv[0],ft_strlen(argv[0]));
		write(2,": command not found\n",20);
	}
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

	if (cmd->next != NULL)
		cmd = multi_redirections(cmd, core, vars);
	vars->file_error = 0;
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
			else if (cmd->args && ft_strchr(cmd->args[0], '/'))
				absolute_path_finder(core, core->env, cmd->args);
			else
				path_finder(vars, core, core->env, cmd->args, 0);
			exit(core->exit_status);
		}
		else
		{
			waitpid(second, &status, 0);
			if (WIFEXITED(status))
				core->exit_status = WEXITSTATUS(status);
			if (vars->filename)
				unlink(vars->filename);
		}
	}
	return ;
}

int	pipe_checker(t_cmdtable *cmd)
{
	t_cmdtable *tmp;
	int i = 0;
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
	int		fd[2];
	t_var	vars;
	pid_t 	second;
	int		status = 0;

	if (pipe_checker(cmd) == 0)
		no_pipe_exe(cmd, core, &vars);
	else if (pipe_checker(cmd) == 1) //Singe Pipe
		single_pipe_exe(cmd, core, &vars);
	else if (pipe_checker(cmd) > 1)
	{
		second = fork();
		if (second == -1)
			return (pipe_error(fd), 1);
		if (second == 0)
			multi_pipe(&vars, cmd, core, core->env);
		else
		{
			waitpid(second, &status, 0);
			if (WIFEXITED(status))
				core->exit_status = WEXITSTATUS(status);
		}
	}
	return (0);
}
