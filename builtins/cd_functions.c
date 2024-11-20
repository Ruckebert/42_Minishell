/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_functions.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:22:11 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/20 10:21:54 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cd_oldpwd(char *old_pwd, t_data *core)
{
	int i = 0;
	
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
	ft_printf("%s\n", core->direct);
	if (chdir(core->direct) == -1)
		core->exit_status = 1;
	envi_update(old_pwd, core);
	free(old_pwd);
}

void	cd_empty(char *old_pwd, t_data *core)
{
	int i;
	i = 0;
		
	free(core->direct);
	while (core->env[i])
	{
		if	(ft_strncmp(core->env[i], "HOME=", 5) == 0)
		{
			core->direct = ft_strdup(core->env[i] + 5);
			if (!core->user)
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

void	normal_cd(char *old_pwd, t_cmdtable *cmd, t_data *core)
{
	free(core->direct);
	core->direct = ft_strdup(cmd->args[1]);
	if (access(core->direct, sizeof(char)) == 0)
	{
		if (chdir(core->direct) == -1)
			core->exit_status = 1;
		envi_update(old_pwd, core);
	}
	else
	{
		write(2, "cd: ", 4);
		write(2, core->direct, ft_strlen(core->direct));
		write(2, ": No such file or directory\n", 29);
		//ft_printf("cd: %s: no such file or directory\n", core->direct);
		core->exit_status = 1;
	}
	free(old_pwd);
}
