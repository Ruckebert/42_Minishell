/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/11 10:26:43 by marsenij         ###   ########.fr       */
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

t_token	*get_args(t_cmdtable *cmd, t_token *token)
{
	t_token	*curr;
	int		i;
	int		strnum;

	if (cmd->args)
		return (token->next);
	i = 0;
	curr = token;
	strnum = get_strnum(curr);
	cmd->args = malloc(sizeof(char *) * (strnum + 1));
	if (!cmd->args)
		return NULL;
	while (i < strnum && curr)
	{
		if (!is_redir(curr) && !is_redir(curr->prev))
		{
			cmd->args[i] = ft_strdup(curr->word);
			if (!cmd->args[i]) {
				while (--i >= 0)
					free(cmd->args[i]);
				free(cmd->args);
				return NULL;
			}
			i++;
		}
		curr = curr->next;
	}
	cmd->args[i] = NULL;
	cmd->has_pipe_after = 0;
	return token;
}


t_token	*add_redir(t_cmdtable *cmd, t_token *curr)
{
	cmd->redir_type = curr->type;
	if (curr->next->type == 30)
		cmd->redir_type = 30;
	cmd->redir = curr->next->word;
	curr = curr->next->next;
	return (curr);
}

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
