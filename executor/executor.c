/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:03:51 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/25 15:04:51 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if (cmd->redir_type != 0 && cmd->redir_type != 10 && cmd->redir_type != 30)
		redirctions(cmd, core, vars, fd);
	//if (core->exit_status == 1)
	//	exit(core->exit_status);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_handler_fd(fd[1], cmd);
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
	//if (cmd->redir_type == 10 || cmd->redir_type == 30)
	//	here_doc(cmd, core, fd[0]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_handler_fd(fd[0], cmd);
	close(fd[0]);
	if (cmd->redir_type != 0 && cmd->redir_type != 10 && cmd->redir_type != 30)
		redirctions(cmd, core, vars, fd);
	//if (core->exit_status == 1)
	//	exit(core->exit_status);
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

//This needs work
t_cmdtable *both_redirections(t_cmdtable *cmd, t_data *core, t_var *vars, int fd)
{
	t_cmdtable *tmp;
	t_cmdtable *output;
	t_cmdtable *input;
	int i = 0;
	int original_stdout = dup(STDOUT_FILENO);
	
	while (cmd)
	{
		if (cmd->redir_type == 1) 
			input = cmd;
		if (cmd->redir_type == 10 || cmd->redir_type == 30)
		{
			input = cmd;
			if (vars->filename != NULL)
				unlink(vars->filename);
			free(vars->filename);
			vars->filename = here_doc_tempfile(cmd, core, fd);
		}
		if (cmd->redir_type == 2 || cmd->redir_type == 20)
		{
			output = cmd;
			if (cmd->next == NULL)
				break ;
			else 
				redirctions(cmd, core, vars, &fd);
			if (dup2(original_stdout, STDOUT_FILENO) == -1)
			{
				close(original_stdout);
				return (NULL);
			}
			close(original_stdout);
		}
		cmd = cmd->next;
	}
	tmp = output;
	i = 0;
	while(output->args[i])
		i++;
	if (input->redir_type == 1)
		output->args[i] = ft_strdup(input->redir);
	else 
		output->args[i] = ft_strdup(vars->filename);
	output->args[i + 1]  = NULL;
	return (output);
}

t_cmdtable *input_redirections(t_cmdtable *cmd, t_data *core, t_var *vars, int fd)
{
	t_cmdtable *input;

	while (cmd)
	{
		if (cmd->redir_type == 1)
			input = cmd;
		if (cmd->redir_type == 10 || cmd->redir_type == 30)
		{
			input = cmd;
			if (vars->filename != NULL)
				unlink(vars->filename);
			//free(vars->filename);
			vars->filename = here_doc_tempfile(cmd, core, fd);
		}
		if (cmd->has_pipe_after == 1)
			break ;
		cmd = cmd->next;
	}
	if (input->redir_type == 10 || input->redir_type == 30)
	{
		input->redir = ft_strdup(vars->filename);
		input->redir_type = 1;
	}
	else
	{
		if (vars->filename != NULL)
			unlink(vars->filename);
	}
	return (input);
}

t_cmdtable *output_redirections(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	t_cmdtable *output = NULL;
	int original_stdout = dup(STDOUT_FILENO);
	
	while (cmd)
	{
		if (cmd->redir_type == 2 || cmd->redir_type == 20)
		{
			output = cmd;
			if (cmd->next == NULL)
				break ;
			else 
				redirctions(cmd, core, vars, fd);
			if (dup2(original_stdout, STDOUT_FILENO) == -1)
			{
				close(original_stdout);
				error_handler_fd(fd[0], cmd);
			}
			close(original_stdout);
		}
		if (cmd->has_pipe_after == 1)
			break ;
		cmd = cmd->next;
	}
	return (output);
}


t_cmdtable *multi_redirections(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	t_cmdtable *tmp;
	tmp = cmd;
	int input = 0;
	int output = 0;
	
	while (cmd)
	{
		if (cmd->redir_type == 1 || cmd->redir_type == 10 || cmd->redir_type == 30)
			input++;
		if (cmd->redir_type == 2 || cmd->redir_type == 20)
			output++;
		if (cmd->has_pipe_after == 1)
			break ;
		cmd = cmd->next;
	}
	cmd = tmp;
	if (input != 0 && output != 0)
		return (both_redirections(cmd, core, vars, *fd));
	else if (input != 0 && output == 0)
		return (input_redirections(cmd, core, vars, *fd));
	else if (input == 0 && output != 0)
		return (output_redirections(cmd, core, vars, fd));
	return (NULL);
}

void	no_pipe_exe(t_cmdtable *cmd, t_data *core, t_var *vars)
{
	int		fd[2];
	pid_t	second;
	int		status = 0;

	if (cmd->next != NULL)
		cmd = multi_redirections(cmd, core, vars, fd);
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

void	child_parent_execution(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
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
		//waitpid(vars->childid, &status, 0);
		//if (WIFEXITED(status))
		//	core->exit_status = WEXITSTATUS(status);
		if (cmd->next->redir_type != 0)
			cmd->next = multi_redirections(cmd->next, core, vars, fd);
		//ft_printf("%s\n", cmd->has_pipe_after);
		//ft_printf("%s\n", cmd->next->args[0]);
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
	if (cmd->redir_type != 0)
	{
		cmd = multi_redirections(cmd, core, vars, fd);
	}
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
		if (vars->filename)
			unlink(vars->filename);
	}	
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
