/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:31:08 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/19 09:59:22 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	finder(int found, int i, char **argv, char **env)
{
	int	argc;
	int	var_len;
	int	env_len;

	var_len = 0;
	env_len = 0;
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

char	**unset_env(t_data *core, char **env, int i, char **argv)
{
	int		new_env;
	int		found;
	char	**temp;

	temp = malloc(((environment_export(core) - i) + 2) * sizeof(char *));
	if (!temp)
		export_malloc_error(core, NULL);
	new_env = 0;
	i = 0;
	while (env[i])
	{
		found = 0;
		found = finder(found, i, argv, env);
		if (!found)
		{
			temp[new_env] = ft_strdup2(env[i]);
			new_env++;
		}
		i++;
	}
	temp[new_env] = NULL;
	return (temp);
}
