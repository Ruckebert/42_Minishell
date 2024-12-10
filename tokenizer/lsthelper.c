/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lsthelper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:23 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/10 11:20:40 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*ft_lstnew(char *word)
{
	t_token	*elem;

	elem = NULL; //malloc(sizeof(t_token));
	if (!elem)
		return (NULL);
	if (word)
		elem->word = word;
	else
		elem = (t_token *){0};
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}

t_token	*ft_lstlast(t_token *lst)
{
	t_token	*temp;

	if (!lst)
		return (NULL);
	temp = lst;
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

void	ft_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*end;

	if (!new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
	}
	else
	{
		end = ft_lstlast(*lst);
		end->next = new;
		new->prev = end;
	}
}

void	ft_lstadd_next(t_token **lst, t_token *new)
{
	t_token	*curr;
	t_token	*tempn;

	curr = *lst;
	if (!new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
	}
	else
	{
		tempn = curr->next;
		curr->next = new;
		curr->next->next = tempn;
		new->prev = curr;
		tempn->prev = new;
	}
}

void	ft_lstdelone(t_token *lst)
{
	t_token	*temp;

	if (lst != NULL)
	{
		temp = lst->prev;
		if (lst->prev)
			lst->prev->next = lst->next;
		if (lst->next)
			lst->next->prev = temp;
		if (lst->word != NULL)
			free(lst->word);
		free(lst);
	}
}
