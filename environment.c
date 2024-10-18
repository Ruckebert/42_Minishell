/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:25:49 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/16 11:21:39 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **free_environment(char **new_env, int i)
{
	while (i > 0)
		free(new_env[--i]);
	free(new_env);
	return (NULL);
}

//To Do: Split Copy_Env 
char **copy_env(char **env, t_data *core)
{
	char **new_env;
	char *temp;
	int	temp_num;
	int count;
	int i;

	if (env == NULL || *env == NULL)
		exit(1);
	
	count = 0;
	while (env[count] != NULL)
		count++;
	
	new_env = malloc((count + 1) * sizeof(char *));
	if (!new_env)
		exit(2);
	core->export_env = malloc((count + 1) * sizeof(char *));
	if (!core->export_env)
		exit(2);
	
	i = 0;
	while (i < count)
	{
		if (ft_strncmp(env[i], "OLDPWD=", 7) == 0)
		{
			new_env[i] = ft_strjoin(ft_substr(env[i], 0, 7), core->direct);
			if (!new_env[i])
				return (free_environment(new_env, i));
		}
		else if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
		{
			temp_num = ft_atoi(env[i] + 6);
			temp_num++;
			temp = ft_itoa(temp_num);
			if (!temp)
				return (free_environment(new_env, i));
			new_env[i] = ft_strjoin(ft_substr(env[i], 0, 6), temp);
			if (!new_env[i])
				return (free_environment(new_env, i));
			free(temp);
		}
		else
		{
			new_env[i] = ft_strdup(env[i]);
			if (!new_env[i])
				return (free_environment(new_env, i));
		}
		if (ft_strncmp(new_env[i], "USER=", 5) == 0)
			core->user = ft_strdup(env[i] + 5); //To Do: IDk if we should free if core->user is null but im pretty its a yes
		if	(ft_strncmp(new_env[i], "HOME=", 5) == 0)
			core->direct = ft_strdup(env[i] + 5);
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}

//To Do: Free if anything fails
void pwd_update(t_data *core)
{
	int i = 0;
	while (core->env[i])
	{
		if (ft_strncmp(core->env[i], "PWD=", 4) == 0)
		{
			core->env[i] = ft_strjoin(ft_substr(core->env[i], 0, 4), core->direct);
			return ;
		}
		i++;
	}
	return ;
}

void	envi_update(char *old_pwd, t_data *core)
{
	int i;

	i = 0;
	core->direct = getcwd(NULL, 0);
	while (core->env[i])
	{
		if (ft_strncmp(core->env[i], "PWD=", 4) == 0)
		{
			core->env[i] = ft_substr(core->env[i], 0, 4);
			core->env[i] = ft_strjoin(core->env[i], core->direct);
		}
		else if (ft_strncmp(core->env[i], "OLDPWD=", 7) == 0)
		{
			core->env[i] = ft_substr(core->env[i], 0, 7);
			core->env[i] = ft_strjoin(core->env[i], old_pwd);
		}
		i++;
	}
	return ;
}
