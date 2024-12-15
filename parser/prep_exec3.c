/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/15 13:15:50 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	allocate_args(t_cmdtable *cmd, int strnum)
{
	cmd->args = malloc(sizeof(char *) * (strnum + 1));
	if (!cmd->args)
		return (-1);
	return (0);
}

int	copy_arguments(t_cmdtable *cmd, t_token *curr, int strnum)
{
	int	i;

	i = 0;
	while (i < strnum && curr)
	{
		if (!is_redir(curr) && !is_redir(curr->prev))
		{
			cmd->args[i] = ft_strdup(curr->word);
			if (!cmd->args[i])
			{
				while (--i >= 0)
					free(cmd->args[i]);
				free(cmd->args);
				return (-1);
			}
			i++;
		}
		curr = curr->next;
	}
	cmd->args[i] = NULL;
	return (0);
}

t_token	*get_args(t_cmdtable *cmd, t_token *token)
{
	t_token	*curr;
	int		strnum;

	if (cmd->args)
		return (token->next);
	curr = token;
	strnum = get_strnum(curr);
	if (allocate_args(cmd, strnum) == -1)
		return (NULL);
	if (copy_arguments(cmd, curr, strnum) == -1)
		return (NULL);
	cmd->has_pipe_after = 0;
	return (token);
}
