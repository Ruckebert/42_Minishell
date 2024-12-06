/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/06 12:50:18 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_redir(t_token	*token)
{
	t_token	*curr;

	curr = token;
	if (curr->type == 1 || curr->type == 2 || curr->type == 10
		|| curr->type == 20 || curr->type == 30)
		return (1);
	return (0);
}

int	is_end(t_token *curr)
{
	if (ft_strncmp(curr->word, "END", 3) == 0
		&& curr->type == 9999)
		return (1);
	return (0);
}

int	is_start(t_token *curr)
{
	if ((ft_strncmp(curr->word, "START", 5) == 0
			&& curr->type == 9999))
		return (1);
	return (0);
}

int	is_pipe(t_token *curr)
{
	if ((ft_strncmp(curr->word, "|", 1) == 0
			&& curr->type == 3))
		return (1);
	return (0);
}

int	has_quote(t_token *curr)
{
	if ((curr->word[0] == '\'' && curr->word[ft_strlen(curr->word) - 1] == '\'')
		|| (curr->word[0] == '\"'
			&& curr->word[ft_strlen(curr->word) - 1] == '\"'))
		return (1);
	return (0);
}
