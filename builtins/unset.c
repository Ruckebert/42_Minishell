/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:31:08 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/18 12:09:32 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int		finder(int found, int i, char **argv, char **env)
{
	int argc;
	int var_len = 0;
	int	env_len = 0;

	argc = 1;
	while (argv[argc])
	{
		var_len = len_env_var(argv, argc);
		env_len = len_env_var(env, i);
		if (var_len == env_len)
		{
			if (ft_strncmp(env[i], argv[argc], var_len) == 0)
			{
				found = 1;
				break ;
			}
		}
		argc++;
	}
	return (found);
}

char	**unset_exo(t_data *core, char **env, int i, char **argv)
{
	int count = environment_export(core);
	char **temp;
	int found = 0;
	
	temp = malloc(((count - i) + 1) * sizeof(char *));
	if (!temp)
		exit(write(1, "Malloc Error", 13));
	i = 0;
	int k = 0;
	while (env[i])
	{
		found = 0;
		found = finder(found, i, argv, env);
		if (!found)
		{
			temp[k] = ft_strdup(env[i]);
			k++;
		}	
		i++;
	}
	temp[k] = NULL;
	return (temp);
}

char	**unset_env(t_data *core, char **env, int i, char **argv)
{
	int new_env;
	int found = 0;
	char **temp;

	temp = malloc(((environment_export(core) - i) +  1) * sizeof(char *));
	if (!temp)
		exit(1);

	new_env = 0;
	i = 0;
	while (env[i])
	{
		found = 0;
		found = finder(found, i, argv, env);
		if (!found)
		{
			temp[new_env] = ft_strdup(env[i]);
			new_env++;
		}
		i++;
	}
	temp[new_env] = NULL;
	return (temp);
}
