/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:07:58 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/11 10:36:49 by aruckenb         ###   ########.fr       */
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

void	create_pwd(t_data *core, int num, char *old_pwd)
{
	char	*argv[3];
	char	**temp;
	int		i;

	i = 0;

	write(1, "1", 1);
	if (core->empty_cd == 1 || core->empty_cd == 0)
	{
		argv[i] = ft_strjoin("OLDPWD=", old_pwd);
		i++;
	}
	if (core->empty_cd == 2 || core->empty_cd == 0)
	{
		argv[i] = ft_strjoin("PWD=", core->direct);
		i++;
	}
	argv[i] = NULL;
	temp = new_exo_env(core->env, argv, num, i);
	simple_free(core->env);
	core->env = temp;
}

void	envi_update(char *old_pwd, t_data *core)
{
	int		i;
	char	*temp;

	i = 0;
	core->empty_cd = 0;
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
			core->empty_cd++;
		}
		else if (ft_strncmp(core->env[i], "OLDPWD=", 7) == 0)
		{
			temp = ft_substr(core->env[i], 0, 7);
			free(core->env[i]);
			core->env[i] = ft_strjoin(temp, old_pwd);
			free(temp);
			core->empty_cd += 2;
		}
		i++;
	}
	if (core->empty_cd != 3)
		create_pwd(core, i, old_pwd);
	return ;
}
