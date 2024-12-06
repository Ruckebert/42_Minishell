/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_delimiter.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/06 13:02:05 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	process_node(t_token *curr)
{
	if (has_quote(curr))
	{
		remove_quotes(curr);
		curr->type = 30;
	}
}

static void	handle_heredoc_merge(t_token *curr)
{
	while (curr->next && curr->next->leading_space == 0
		&& !is_redir(curr->next) && curr->next->type != 3)
	{
		process_node(curr);
		if (curr->next->type == 9)
			curr->type = 30;
		process_node(curr->next);
		fuse_node_with_next(curr);
	}
}

void	handle_heredoc_delimiter(t_token *token)
{
	t_token	*curr;

	curr = token;
	while (curr)
	{
		if (!is_start(curr) && is_heredoc(curr->prev))
		{
			if (curr->next->leading_space != 0)
				process_node(curr);
			handle_heredoc_merge(curr);
		}
		curr = curr->next;
	}
}
