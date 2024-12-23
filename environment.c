/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 10:25:49 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/20 12:48:39 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**free_environment(char **new_env)
{
	simple_free(new_env);
	return (NULL);
}

char	**shellvl(int i, char **env, char **new_env)
{
	char	*temp;
	char	*sub_temp;
	int		temp_num;

	temp_num = ft_atoi(env[i] + 6);
	temp = ft_itoa(temp_num);
	if (!temp)
		return (NULL);
	sub_temp = ft_substr(env[i], 0, 6);
	if (!sub_temp)
		return (free(temp), NULL);
	new_env[i] = ft_strjoin(sub_temp, temp);
	if (!new_env[i])
		return (free(temp), free(sub_temp), NULL);
	free(temp);
	free(sub_temp);
	return (new_env);
}

void	core_direct_user(t_data *core, char **new_env, char **env, int i)
{
	if (ft_strncmp(new_env[i], "HOME=", 5) == 0)
	{
		core->direct = ft_strdup(env[i] + 5);
		if (!core->direct)
		{
			free_environment(new_env);
			return ;
		}
	}
}

char	**environment_copy(char **env, char **new_env, t_data *core, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
		{
			if (shellvl(i, env, new_env) == NULL)
				return (free(core->export_env), free_environment(new_env));
		}
		else
		{
			new_env[i] = ft_strdup(env[i]);
			if (!new_env[i])
				return (free(core->export_env), free_environment(new_env));
		}
		core_direct_user(core, new_env, env, i);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

char	**copy_env(char **env, t_data *core)
{
	char	**new_env;
	int		count;

	new_env = NULL;
	if (env == NULL || *env == NULL)
		exit(1);
	count = 0;
	while (env[count] != NULL)
		count++;
	new_env = ft_calloc((count + 1), sizeof(char *));
	if (!new_env)
		exit(1);
	core->export_env = ft_calloc((count + 1), sizeof(char *));
	if (!core->export_env)
	{
		free(new_env);
		exit(1);
	}
	return (environment_copy(env, new_env, core, count));
}
