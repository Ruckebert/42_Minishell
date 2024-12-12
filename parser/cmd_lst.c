/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/12 12:19:19 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmdtable	*ft_lstnew_cmd(char *redir, int type)
{
	t_cmdtable	*elem;

	elem = malloc(sizeof(t_cmdtable));
	if (!elem)
		return (NULL);
	elem->redir = redir;
	elem->redir_type = type;
	elem->has_pipe_after = 0;
	elem->isbuiltin = 0;
	elem->args = NULL;
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}

t_cmdtable	*ft_lstlast_cmd(t_cmdtable *lst)
{
	t_cmdtable	*temp;

	if (!lst)
		return (NULL);
	temp = lst;
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

void	ft_lstadd_back_cmd(t_cmdtable **lst, t_cmdtable *new)
{
	t_cmdtable	*end;

	if (!new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
	}
	else
	{
		end = ft_lstlast_cmd(*lst);
		end->next = new;
		new->prev = end;
	}
}
