/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:05:35 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/06 14:06:12 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	dup_malloc_count(t_cmdtable *cmd, int j, int k, int sub)
{
	int	var_len;

	var_len = 0;
	while (cmd->args[j])
	{
		var_len = len_env_var(cmd->args, j);
		k = 0;
		while (cmd->args[k])
		{
			if (k == j)
				break ;
			if (ft_strncmp(cmd->args[k], cmd->args[j], var_len) == 0)
			{
				if (ft_strcmp(cmd->args[k], cmd->args[j]) != 0)
				{
					sub++;
					break ;
				}
			}
			k++;
		}
		j++;
	}
	j -= sub;
	return (j);
}

int	cmd_args_count(t_cmdtable *cmd)
{
	int	count;

	count = 0;
	while (cmd->args[count])
		count++;
	return (count);
}