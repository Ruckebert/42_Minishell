/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freefuncs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:07:58 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/15 16:06:01 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    free_token_list(t_token *head)
{
	t_token	*tmp;

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

void free_cmdtable(t_cmdtable **head)
{
	t_cmdtable	*tmp;
	t_cmdtable	*next;
	int			i;

	tmp = *head;
	while (tmp != NULL)
	{
		next = tmp->next;

		if (tmp->args != NULL)
		{
			for (i = 0; tmp->args[i]; i++)
			{
				free(tmp->args[i]);
				tmp->args[i] = NULL;
			}
			free(tmp->args);
			tmp->args = NULL;
		}
		if (tmp->redir)
		{
			free(tmp->redir);
		}
		free(tmp);
		tmp = NULL;
		tmp = next;
	}
	*head = NULL;
}