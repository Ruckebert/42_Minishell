/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finders.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:36:24 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/16 11:12:14 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	path_checker(char **envp)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
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

void	path_finder_end_checker(t_var *vars, t_data *core,
	char **envp, char **argv)
{
	if (ft_strlen(envp[vars->store_env]) <= 5)
	{
		free_split(vars->store);
		absolute_path_finder(core, envp, argv);
	}
	else if (path_checker(envp) == 1 && (access(argv[0], R_OK) == 0))
		execve(argv[0], argv, envp);
	else
	{
		write(2, argv[0], ft_strlen(argv[0]));
		write(2, ": command not found\n", 20);
	}
	free_split(vars->store);
	core->exit_status = 127;
	free_exit(core);
	exit(core->exit_status);
}

void	path_finder_exe_lop(t_var *vars, char **envp, char **argv, int i)
{
	vars->comm = ft_strjoin(vars->store[i], "/");
	if (vars->comm == NULL)
		error_handler(NULL);
	vars->full_comm = ft_strjoin(vars->comm, argv[0]);
	if (vars->full_comm == NULL)
		error_handler(NULL);
	free(vars->comm);
	execve(vars->full_comm, argv, envp);
	free(vars->full_comm);
}

void	path_finder(t_var *vars, t_data *core, char **envp, char **argv)
{
	int	i;

	i = 0;
	if (argv == NULL)
		return ;
	while (envp[i] && !ft_strnstr(envp[i], "PATH", 4))
		i++;
	if (!envp[i])
		absolute_path_finder(core, envp, argv);
	vars->store = ft_split(envp[i] + 5, ':');
	if (!vars->store)
		error_handler_split(vars->store);
	vars->store_env = i;
	i = -1;
	while (vars->store[++i])
		path_finder_exe_lop(vars, envp, argv, i);
	path_finder_end_checker(vars, core, envp, argv);
}

void	absolute_path_finder(t_data *core, char **envp, char **argv)
{
	struct stat	filestat;

	if (access(argv[0], R_OK) == 0)
		execve(argv[0], argv, envp);
	if (stat(argv[0], &filestat) == 0)
	{
		write(2, argv[0], ft_strlen(argv[0]));
		write(2, ": Permission denied\n", 20);
		core->exit_status = 126;
	}
	else
	{
		write(2, argv[0], ft_strlen(argv[0]));
		write(2, ": No such file or directory\n", 28);
		core->exit_status = 127;
	}
	free_exit(core);
	exit(core->exit_status);
}
