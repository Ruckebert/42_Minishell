/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/15 17:04:36 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmdtable	*initialize_cmd_table(t_token **curr)
{
	t_cmdtable	*cmd;
	t_cmdtable	*newcmd;

	cmd = NULL;
	newcmd = ft_lstnew_cmd(NULL, 0);
	ft_lstadd_back_cmd(&cmd, newcmd);
	if (is_start(*curr))
		*curr = (*curr)->next;
	return (cmd);
}

t_token	*process_redir(t_cmdtable **newcmd, t_cmdtable **cmd, t_token *curr)
{
	if ((*newcmd)->redir)
	{
		*newcmd = ft_lstnew_cmd(NULL, 0);
		ft_lstadd_back_cmd(cmd, *newcmd);
		copy_args(*newcmd);
	}
	return (add_redir(*newcmd, curr));
}

t_token	*process_pipe(t_cmdtable **newcmd, t_cmdtable **cmd, t_token *curr)
{
	*newcmd = ft_lstnew_cmd(NULL, 0);
	ft_lstadd_back_cmd(cmd, *newcmd);
	(*newcmd)->prev->has_pipe_after = 1;
	return (curr->next);
}

void	*process_tokens(t_cmdtable **cmd, t_token *curr)
{
	t_cmdtable	*newcmd;

	newcmd = *cmd;
	while (!is_end(curr))
	{
		if (is_redir(curr))
			curr = process_redir(&newcmd, cmd, curr);
		else if (is_pipe(curr))
			curr = process_pipe(&newcmd, cmd, curr);
		else
			curr = get_args(newcmd, curr);
		if (curr == NULL)
			return (NULL);
	}
	return ((void *)1);
}

t_cmdtable	*prep_nodes_for_exec(t_token *token, t_data *core)
{
	t_cmdtable	*cmd;
	t_token		*curr;

	cmd = (t_cmdtable *){0};
	if (!token)
		return (NULL);
	curr = token;
	cmd = initialize_cmd_table(&curr);
	if (process_tokens(&cmd, curr) == NULL)
	{
		free_token_list(token);
		free_exit(core);
		exit(core->exit_status);
	}
	find_builtins(cmd);
	if (token)
		free_token_list(token);
	return (cmd);
}
