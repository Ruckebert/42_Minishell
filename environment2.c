/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:07:58 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/10 15:13:45 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd_update(t_data *core)
{
	int		i;
	char	*temp;

	i = 0;
	while (core->env[i])
	{
		if (ft_strncmp(core->env[i], "PWD=", 4) == 0)
		{
			temp = ft_substr(core->env[i], 0, 4);
			free(core->env[i]);
			core->env[i] = ft_strjoin(temp, core->direct);
			if (!core->env[i])
				free_environment(core->env, i);
			free(temp);
		}
		i++;
	}
	return ;
}

void	envi_update(char *old_pwd, t_data *core)
{
	int		i;
	char	*temp;

	i = 0;
	free(core->direct);
	core->direct = getcwd(NULL, 0);
	while (core->env[i])
	{
		if (ft_strncmp(core->env[i], "PWD=", 4) == 0)
		{
			temp = ft_substr(core->env[i], 0, 4);
			free(core->env[i]);
			core->env[i] = ft_strjoin(temp, core->direct);
			free(temp);
		}
		else if (ft_strncmp(core->env[i], "OLDPWD=", 7) == 0)
		{
			temp = ft_substr(core->env[i], 0, 7);
			free(core->env[i]);
			core->env[i] = ft_strjoin(temp, old_pwd);
			free(temp);
		}
		i++;
	}
	return ;
}
