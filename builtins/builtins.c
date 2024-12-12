/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:26:46 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/12 12:50:28 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	multi_array_counter(char **argc)
{
	int	i;

	i = 0;
	while (argc[i])
		i++;
	return (i);
}

void	cd_com(t_cmdtable *cmd, t_data *core)
{
	char	*old_pwd;

	core->exit_status = 0;
	old_pwd = NULL;
	old_pwd = getcwd(NULL, 0);
	if (multi_array_counter(cmd->args) >= 3)
	{
		write(2, "cd: too many arguments\n", 23);
		core->exit_status = 1;
		free(old_pwd);
		return ;
	}
	if (cmd->args[1] == NULL)
	{
		cd_empty(old_pwd, core);
		return ;
	}
	else if (cmd->args[1][0] == '-')
	{
		cd_oldpwd(old_pwd, core);
		return ;
	}
	normal_cd(old_pwd, cmd, core);
	return ;
}

void	pwd(t_data *core)
{
	free(core->direct);
	core->direct = getcwd(NULL, 0);
	if (!core->direct)
		return ;
	core->exit_status = 0;
	ft_printf("%s\n", core->direct);
}

void	env(t_data *core)
{
	int	i;
	int	j;

	i = 0;
	while (core->env[i] != NULL)
	{
		j = 0;
		while (core->env[i][j])
		{
			if (core->env[i][j] == '=')
			{
				ft_printf("%s\n", core->env[i]);
				break ;
			}
			j++;
		}
		i++;
	}
	return ;
}

void	insert_new_env(t_data *core, char **temp, char **temp_env)
{
	simple_free(core->export_env);
	core->export_env = temp;
	simple_free(core->env);
	core->env = temp_env;
}

void	export_malloc_error(t_data *core, char **temp)
{
	if (temp)
		simple_free(temp);
	write(2, "Error: Failed Malloc\n", 21);
	free_exit(core);
	exit(1);
}
void	export(t_cmdtable *cmd, t_data *core)
{
	int		i;
	int		count;
	char	**temp;
	char	**temp_env;

	count = environment_export(core);
	bubble_sort(core);
	i = count + 1;
	count = cmd_args_count(cmd);
	if (count == 1)
		print_exo_env(core, 0);
	else if (count > 1)
	{
		count = dup_malloc_count(cmd, 1, 0, 0);
		temp = new_exo_env(core->export_env, cmd->args, i, count);
		if (!temp)
		{
			core->exit_status = 1;
			return ;
		}
		temp_env = new_exo_env(core->env, cmd->args, i, count);
		if (!core->env)
			export_malloc_error(core, temp);
		insert_new_env(core, temp, temp_env);
	}
}

void	unset(t_cmdtable *cmd, t_data *core)
{
	char	**temp;
	char	**temp_env;
	int		i;

	i = 0;
	core->exit_status = 0;
	temp = unset_env(core, core->export_env, i, cmd->args);
	temp_env = unset_env(core, core->env, i, cmd->args);
	simple_free(core->export_env);
	core->export_env = temp;
	simple_free(core->env);
	core->env = temp_env;
}

void	echo_cmd(t_cmdtable *cmd, t_data *core, int i)
{
	int	no;
	int	j;

	i = 1;
	j = 1;
	no = 0;
	if (cmd->args[i] == NULL)
		no = 0;
	else if (ft_strncmp(cmd->args[i], "-n", 2) == 0)
	{
		while (cmd->args[i][j])
		{
			if (cmd->args[i][j] == 'n')
				no = 1;
			else
			{
				no = 0;
				break ;
			}
			j++;
		}
		if (no == 1)
			i = second_no(&no, i, cmd);
	}
	echo_exit(i, no, cmd, core);
}

void	free_exit(t_data *core)
{
	if(core->env != NULL)
		simple_free(core->env);
	if (core->export_env != NULL)
		simple_free(core->export_env);
	free(core->direct);
	free(core->line);
	if (core->cmd != NULL)
		free_cmdtable(&core->cmd);
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}

void	exit_com(t_data *core)
{
	int		i;
	int		j;
	char	*temp;

	i = 1;
	j = 0;
	temp = NULL;
	if (core->cmd->args[1])
	{
		temp = ft_strtrim(core->cmd->args[1], " ");
		core->cmd->args[1] = ft_strdup(temp);
		free(temp);
	}
	j = exit_loop(core, i, j);
	if (exit_error_handler(j, core) == 1)
		return ;
	free_exit(core);
	exit(core->exit_status);
}
