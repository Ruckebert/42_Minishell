/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:26:46 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/15 09:56:53 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
