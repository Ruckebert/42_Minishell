/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 10:03:51 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/06 12:48:42 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t g_interrupt_received;

int	path_checker(char **envp)
{
	int i = 0;
	int	j = 0;

	while (envp[i] && !ft_strnstr(envp[i], "PATH", 4))
		i++;
	while (envp[i][j])
	{
		if (envp[i][j] == ':' && j == 5)
			return (1);
		else if (envp[i][j] == ':' && j == (int)ft_strlen(envp[i]) - 1)
			return (1);
		else if (envp[i][j] == ':' && envp[i][j - 1] == ':')
			return (1);
		j++;
	}
	return (0);
}

void	path_finder_end_checker(t_var *vars, t_data *core, char **envp, char **argv)
{
	if (ft_strlen(envp[vars->store_env]) <= 5)
		absolute_path_finder(core, envp, argv);
	else if (path_checker(envp) == 1 && (access(argv[0], R_OK) == 0))
		execve(argv[0], argv, envp);
	else
	{
		write(2,argv[0],ft_strlen(argv[0]));
		write(2,": command not found\n",20);
	}
	free_split(vars->store);
	core->exit_status = 127;
	exit(core->exit_status);
}

void	path_finder(t_var *vars, t_data *core, char **envp, char **argv, int i)
{
	if (argv == NULL)
		return ;
	while (envp[i] && !ft_strnstr(envp[i], "PATH", 4))
		i++;
	if (!envp[i])
		absolute_path_finder(core, envp, argv);
	vars->store = ft_split(envp[i] + 5, ':');
	if (!vars->store)
		error_handler();
	vars->store_env = i;
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
	path_finder_end_checker(vars, core, envp, argv);
}

void	absolute_path_finder(t_data *core, char **envp, char **argv)
{
	struct stat fileStat;
	
	if (access(argv[0], R_OK) == 0)
		execve(argv[0], argv, envp);
	if (stat(argv[0], &fileStat) == 0)
	{
		write(2,argv[0],ft_strlen(argv[0]));
		write(2, ": Permission denied\n", 20);
		core->exit_status = 126;
	}
	else
	{
		write(2,argv[0],ft_strlen(argv[0]));
		write(2, ": No such file or directory\n", 28);
		core->exit_status = 127;
	}
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
	exit(1); //Free everything and exit.
}


//Work in progress
void	here_doc_file_del(char **files)
{
	int i;

	i = 0;
	if (files)
	{
		while (files[i])
		{
			unlink(files[i]);
			i++;
		}
	}
}

void	here_doc_creator(t_cmdtable *cmd, t_data *core, char **files)
{
	int i;
	t_cmdtable *current_cmd;
	
	i = 0;
	current_cmd = cmd;
	if (here_doc_counter(cmd) != 0)
	{
		files = ft_calloc(here_doc_counter(cmd), sizeof(char *));
		while (cmd)
		{
			if (cmd->redir_type == 10 || cmd->redir_type == 30)
			{
				cmd->redir = ft_strdup(here_doc_tempfile(cmd, core, STDIN_FILENO));
				if (g_interrupt_received != 0)
				{
					files[i] = ft_strdup(cmd->redir);
					here_doc_file_del(files);
					return ;
				}
				cmd->redir_type = 1;
				files[i] = ft_strdup(cmd->redir);
				i++;
			}
			cmd = cmd->next;
		}
		cmd = current_cmd;
	}	
}

void	execution_pro(t_cmdtable *cmd, t_data *core, t_var *vars, int fd[2])
{
	if (cmd->redir_type != 0)
		redirctions(cmd, core, vars, fd);
	if (cmd->isbuiltin == 1)
		echo_cmd(cmd, core, 0);
	else if (cmd->args && ft_strchr(cmd->args[0], '/'))
		absolute_path_finder(core, core->env, cmd->args);
	else
		path_finder(vars, core, core->env, cmd->args, 0);
	exit(core->exit_status);
}

void	no_pipe_status(char **files, int status, t_data *core, pid_t second)
{
	setup_signal_handler(SIGINT, sig_handleINT_parent2);
	waitpid(second, &status, 0);
	if (WIFEXITED(status))
		core->exit_status = WEXITSTATUS(status);
	here_doc_file_del(files);
}

void	no_pipe_exe(t_cmdtable *cmd, t_data *core, t_var *vars, int status)
{
	int		fd[2];
	pid_t	second;
	char	**files;
	
	files = NULL;
	here_doc_creator(cmd, core, files);
	if (cmd->next != NULL)
		cmd = multi_redirections(cmd, core, vars);
	if (cmd->isbuiltin != 0 && cmd->isbuiltin != 1)
		builtin_cmds(cmd, core);
	else
	{
		second = fork();
		if (second == -1)
			pipe_error(fd);
		if (second == 0)
			execution_pro(cmd, core, vars, fd);
		else
			no_pipe_status(files, status, core, second);
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
	int		status;

	status = 0;
	if (pipe_checker(cmd) == 0)
		no_pipe_exe(cmd, core, &vars, status);
	else if (pipe_checker(cmd) == 1)
		single_pipe_exe(cmd, core, &vars);
	else if (pipe_checker(cmd) > 1)
	{
		second = fork();
		if (second == -1)
			return (pipe_error(fd), 1);
		if (second == 0)
			multi_pipe(&vars, cmd, core);
		else
		{
			waitpid(second, &status, 0);
			if (WIFEXITED(status))
				core->exit_status = WEXITSTATUS(status);
		}
	}
	return (0);
}
