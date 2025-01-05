/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freefuncs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:07:58 by aruckenb          #+#    #+#             */
/*   Updated: 2025/01/05 09:26:27 by aruckenb         ###   ########.fr       */
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

void	free_cmdtable_node(t_cmdtable *node)
{
	int	i;

	if (!node)
		return ;
	if (node->args != NULL)
	{
		i = 0;
		while (node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
		node->args = NULL;
	}
	if (node->redir)
	{
		free(node->redir);
		node->redir = NULL;
	}
	free(node);
	node = NULL;
}

void	free_cmdtable(t_cmdtable **head)
{
	t_cmdtable	*tmp;
	t_cmdtable	*next;

	if (!head || !*head)
		return ;
	tmp = *head;
	while (tmp != NULL)
	{
		next = tmp->next;
		free_cmdtable_node(tmp);
		tmp = next;
	}
	*head = NULL;
}

char	**free_environment(char **new_env)
{
	simple_free(new_env);
	return (NULL);
}
