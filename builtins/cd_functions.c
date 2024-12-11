/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_functions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:22:11 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/11 10:32:07 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cd_oldpwd(char *old_pwd, t_data *core)
{
	int	i;

	i = 0;
	while (core->env[i])
	{
		if (ft_strncmp(core->env[i], "OLDPWD=", 7) == 0)
		{
			free(core->direct);
			core->direct = ft_strdup(core->env[i] + 7);
			break ;
		}
		i++;
	}
	if (core->env[i] == NULL)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		return ;
	}
	ft_printf("%s\n", core->direct);
	if (chdir(core->direct) == -1)
		core->exit_status = 1;
	envi_update(old_pwd, core);
	free(old_pwd);
}

void	cd_empty(char *old_pwd, t_data *core)
{
	int	i;

	i = 0;
	while (core->env[i])
	{
		if (ft_strncmp(core->env[i], "HOME=", 5) == 0)
		{
			free(core->direct);
			core->direct = ft_strdup(core->env[i] + 5);
			if (!core->direct)
				return ;
			break ;
		}
		i++;
	}
	if (chdir(core->direct) == -1)
		core->exit_status = 1;
	envi_update(old_pwd, core);
	free(old_pwd);
}

void	print_cd_error_msg(t_cmdtable *cmd, t_data *core)
{
	struct stat	filestat;

	if (stat(cmd->args[1], &filestat) == 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(core->direct, 2);
		ft_putstr_fd(": Permission deined\n", 2);
	}
	else
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(core->direct, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
	}
}

void	normal_cd(char *old_pwd, t_cmdtable *cmd, t_data *core)
{
	free(core->direct);
	core->direct = ft_strdup(cmd->args[1]);
	if (access(core->direct, sizeof(char)) == 0)
	{
		if (chdir(core->direct) == -1)
			core->exit_status = 1;
		if (old_pwd == NULL && core->empty_cd != 1)
		{
			ft_putstr_fd("cd: error: cannot access parent directory\n", 2);
			core->empty_cd++;
		}
		else if (core->empty_cd == 1)
			core->empty_cd = 0;
		envi_update(old_pwd, core);
	}
	else
	{
		print_cd_error_msg(cmd, core);
		core->exit_status = 1;
	}
	free(old_pwd);
}
