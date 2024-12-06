/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fuse_node_word.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/06 12:50:47 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_links(t_token *curr, t_token *discard)
{
	t_token	*next_node;

	next_node = discard->next;
	if (next_node)
		next_node->prev = curr;
	curr->next = next_node;
	free(discard->word);
	free(discard);
}

static void	handle_space_case(t_token *curr, char *oldword, char *new_word)
{
	char	*space_pos;
	int		firstspace_len;
	char	*trimmed_part;

	space_pos = ft_strchr(new_word, ' ');
	firstspace_len = (space_pos - new_word);
	trimmed_part = malloc(firstspace_len + 1);
	if (!trimmed_part)
		return ;
	ft_strlcpy(trimmed_part, new_word, firstspace_len + 1);
	curr->word = ft_strjoin(oldword, trimmed_part);
	free(trimmed_part);
}

void	fuse_node_word(t_token *curr, char *new_word)
{
	char	*oldword;

	if (!curr || !curr->next)
		return ;
	oldword = curr->word;
	update_links(curr, curr->next);
	if (ft_strchr(new_word, ' '))
		handle_space_case(curr, oldword, new_word);
	else
		curr->word = ft_strjoin(oldword, new_word);
	free(oldword);
}
