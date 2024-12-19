/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_exec_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/19 17:22:44 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_all(void)
{
	free_token_list(address_getter_token(NULL));
	free_exit_no_cmd(address_getter(NULL));
	exit(1);
}

void	copy_args(t_cmdtable *cmd)
{
	int		strnum;

	strnum = 0;
	if (cmd->prev->args)
	{
		while (cmd->prev->args[strnum])
			strnum++;
		cmd->args = malloc(sizeof(char *) * (strnum + 1));
		if (!cmd->args)
			free_all();
		strnum = 0;
		while (cmd->prev->args[strnum])
		{
			cmd->args[strnum] = ft_strdup(cmd->prev->args[strnum]);
			if (!cmd->args[strnum])
				free_all();
			strnum++;
		}
		cmd->args[strnum] = NULL;
	}
}

void	init_freethis(t_token *curr)
{
	curr->freethis = malloc (2 * sizeof(char *));
	if (!curr->freethis)
	{
		free_token_list(address_getter_token(NULL));
		free_exit(address_getter(NULL));
		exit(1);
	}
	curr->freethis[0] = curr->word;
	curr->freethis[1] = NULL;
	curr->freethis_num = 1;
}
