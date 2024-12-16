/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/15 16:59:04 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_builtin_status(t_cmdtable *cmd)
{
	if (cmd->args && cmd->args[0] != NULL)
	{
		if (!ft_strcmp(cmd->args[0], "echo"))
			cmd->isbuiltin = 1;
		else if (!ft_strcmp(cmd->args[0], "cd"))
			cmd->isbuiltin = 2;
		else if (!ft_strcmp(cmd->args[0], "pwd"))
			cmd->isbuiltin = 3;
		else if (!ft_strcmp(cmd->args[0], "export"))
			cmd->isbuiltin = 4;
		else if (!ft_strcmp(cmd->args[0], "unset"))
			cmd->isbuiltin = 5;
		else if (!ft_strcmp(cmd->args[0], "env"))
			cmd->isbuiltin = 6;
		else if (!ft_strcmp(cmd->args[0], "exit"))
			cmd->isbuiltin = 7;
		else
			cmd->isbuiltin = 0;
	}
	else
		cmd->isbuiltin = 0;
}

void	find_builtins(t_cmdtable *cmd)
{
	t_cmdtable	*curr;

	curr = cmd;
	while (curr != NULL)
	{
		set_builtin_status(curr);
		curr = curr->next;
	}
}
