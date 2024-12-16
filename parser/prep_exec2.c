/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/15 17:23:08 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_strnum(t_token *curr)
{
	int	i;

	i = 0;
	while (curr->type != 3 && !is_end(curr))
	{
		if (is_redir(curr))
		{
			if (is_end(curr->next->next))
				break ;
			curr = curr->next->next;
			continue ;
		}
		i++;
		curr = curr->next;
	}
	return (i);
}

int	get_args_loop(t_cmdtable *cmd, t_token *curr, int strnum)
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
	cmd->has_pipe_after = 0;
	return (0);
}

t_token	*get_args(t_cmdtable *cmd, t_token *token)
{
	t_token	*curr;
	int		strnum;
	int		error;

	if (cmd->args)
		return (token->next);
	curr = token;
	strnum = get_strnum(curr);
	cmd->args = malloc(sizeof(char *) * (strnum + 1));
	if (!cmd->args)
		return (NULL);
	error = get_args_loop(cmd, curr, strnum);
	if (error == -1)
		return (NULL);
	return (token);
}

t_token	*add_redir(t_cmdtable *cmd, t_token *curr)
{
	cmd->redir_type = curr->type;
	if (curr->next->type == 30)
		cmd->redir_type = 30;
	cmd->redir = ft_strdup(curr->next->word);
	curr = curr->next->next;
	return (curr);
}
