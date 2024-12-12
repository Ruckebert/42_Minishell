/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_exec_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/12 12:26:46 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	copy_args(t_cmdtable *cmd)
{
	int	strnum;

	strnum = 0;
	if (cmd->prev->args)
	{
		while (cmd->prev->args[strnum])
			strnum++;
		cmd->args = malloc(sizeof(char *) * (strnum + 1));
		strnum = 0;
		while (cmd->prev->args[strnum])
		{
			cmd->args[strnum] = ft_strdup(cmd->prev->args[strnum]);
			strnum++;
		}
		cmd->args[strnum] = NULL;
	}
}
