/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fuse_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/18 18:06:30 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fuse_node_with_next(t_token *curr)
{
	t_token	*next_node;
	t_token	*discard;
	char	*oldword;

	if (!curr || !curr->next)
		return ;
	oldword = curr->word;
	next_node = curr->next->next;
	if (next_node)
		next_node->prev = curr;
	discard = curr->next;
	curr->next = next_node;
	curr->word = ft_strjoin(oldword, discard->word);
	free(oldword);
	free(discard->word);
	free(discard);
}

void	fuse_all_0space_nodes(t_token *token)
{
	t_token	*curr;

	curr = token;
	while (curr)
	{
		if (is_fusable(curr) && curr->leading_space == 0
			&& curr->prev->type != 9999
			&& !is_redir(curr) && !is_redir(curr->prev))
		{
			curr->prev->type = curr->type;
			curr = curr->prev;
			fuse_node_with_next(curr);
		}
		if (is_fusable(curr) && curr->next->type != 9999
			&& curr->next->leading_space == 0
			&& !is_redir(curr) && !is_redir(curr->next))
		{
			fuse_node_with_next(curr);
			curr = curr->prev;
		}
		curr = curr->next;
	}
}

void	remove_empty_quotes(t_token *token)
{
	t_token	*curr;

	curr = token;
	while (curr)
	{
		if ((curr->type == 5 || curr->type == 4) && ft_strlen(curr->word) == 2)
		{
			free(curr->word);
			curr->word = ft_strdup("");
			if (!curr->word)
				free_all();
			curr->type = 9;
		}
		curr = curr->next;
	}
}

char	*parse_var_name(t_token *curr)
{
	char	*start;
	int		len;
	char	*res;

	len = 0;
	start = ft_strchr(curr->word, '$');
	start++;
	if (*start != ' ')
	{
		while (start[len] != ' ' && start[len]
			!= '\0' && start[len] != '\'' && start[len] != '/')
			len++;
		res = malloc(len + 1);
		if (!res)
			free_all();
		ft_strlcpy(res, start, len + 1);
		return (res);
	}
	else
		return (NULL);
}
