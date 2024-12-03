/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:34:16 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/02 11:49:03 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	while ((*s1 != '\0') && (*s2 != '\0'))
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	if ((*s1 != *s2))
		return (*s1 - *s2);
	return (0);
}

int	len_env_var(char **argv, int j)
{
	int i;
	
	i = 0;
	while (argv[j][i])
	{
		if (argv[j][i] == '=')
			break ;
		i++;
	}
	return (i);
}

int		environment_export(t_data *core)
{
	int count;

	count = 0;
	if (core->export_env[0] == NULL)
	{
		while (core->env[count])
		{
			core->export_env[count] = ft_strdup(core->env[count]);
			count++;
		}
		core->export_env[count] = NULL;
	}
	else
	{
		while (core->env[count])
			count++;
	}
	return (count);
}
