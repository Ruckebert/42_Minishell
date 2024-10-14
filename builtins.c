/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:26:46 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/11 15:29:14 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// All Builtins Manipulate the Environment that is why we have to build them seperately. :0 

void	cd_com(t_data *core)
{
	
	/*if (ft_strncmp(core->line, "cd -", 4) == 0) //To Do: Update the OldPWD
	{
		int i = 0;
		while (core->env[i])
		{
			if (ft_strncmp(core->env[i], "OLDPWD=", 8) == 0)
			{
				core->direct = ft_strdup(core->line + 8);
				break ;
			}
			i++;	
		}
		chdir(core->direct);
		return ;
	}*/
	
	core->direct = ft_strdup(core->line + 3);
	if (access(core->direct, sizeof(char)) == 0)
		chdir(core->direct);
	else
		ft_printf("cd: no such file or directory: %s\n", core->direct);
	return ;
}

void	pwd(t_data *core)
{
	core->direct = getcwd(NULL, 0);
	ft_printf("%s\n", core->direct);
	
	/*int i = 0; To Do: Update the PWD with the current Directory
	while (core->env[i])
	{
		if (ft_strncmp(core->env[i], "PWD=", 4) == 0)
			core->user = ft_strdup(core->env[i] + 4);
		i++;
	}*/
	
}

void	env(t_data *core)
{
	int i;
	
	i = 0;
	while (core->env[i] != NULL)
	{
		ft_printf("%s\n",core->env[i]);
		i++;
	}
	return ;
}

void	export(t_data *core)
{
	env(core); //To do: This occurs if the export does not have any arguments
}

void	unset()
{
	
}
