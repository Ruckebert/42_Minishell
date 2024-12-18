/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freefuncs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:07:58 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/18 16:10:53 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token_list(t_token *head)
{
	t_token	*tmp;


	while (head && head->prev != NULL)
	{
		head = head->prev;
	}
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
		if (tmp->word)
		{
			free (tmp->word);
			tmp->word = NULL;
		}
		free (tmp);
		tmp = NULL;
	}
}

void	free_cmdtable(t_cmdtable **head)
{
	t_cmdtable	*tmp;
	t_cmdtable	*next;
	int			i;

	if (!head)
		return;
	tmp = *head;
	while (tmp != NULL)
	{
		next = tmp->next;
		if (tmp->args != NULL)
		{
			i = -1;
			while (tmp->args[++i])
				free(tmp->args[i]);
			free(tmp->args);
			tmp->args = NULL;
		}
		if (tmp->redir)
		{
			free(tmp->redir);
			tmp->redir = NULL;
		}
		free(tmp);
		tmp = NULL;
		tmp = next;
	}
	*head = NULL;
}
