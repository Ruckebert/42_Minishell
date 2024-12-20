/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 11:04:17 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/20 11:08:02 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	reverse_free(int i, char **temp)
{
	while (i >= 0)
	{
		free(temp[i]);
		i--;
	}
	free(temp);
	exit(2);
}

int	environment_export(t_data *core)
{
	int		count;

	count = 0;
	if (core->export_env[0] == NULL)
	{
		while (core->env[count])
		{
			core->export_env[count] = ft_strdup2(core->env[count]);
			count++;
		}
	}
	else
	{
		while (core->env[count])
		{
			free(core->export_env[count]);
			core->export_env[count] = ft_strdup2(core->env[count]);
			count++;
		}
	}
	core->export_env[count] = NULL;
	return (count);
}
