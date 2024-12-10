/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenhelper2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:23 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/10 14:14:50 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	combine_double_redirect(t_token	*token, t_data *core)
{
	t_token	*curr;

	curr = token;
	while (curr && curr->next)
	{
		if (curr->type == curr->next->type)
		{
			if (!ft_strncmp(curr->word, "<\0", 2)
				&& !ft_strncmp(curr->next->word, "<\0", 2)
				&& (curr->next->leading_space == 0))
				substitute_redir(curr, "<<\0", token, core);
			else if (!ft_strncmp(curr->word, ">\0", 2)
				&& !ft_strncmp(curr->next->word, ">\0", 2)
				&& (curr->next->leading_space == 0))
				substitute_redir(curr, ">>\0", token, core);
		}
		curr = curr->next;
	}
}

void	make_start_token(t_token **token, t_data *core)
{
	t_token	*newtoken;

	newtoken = ft_lstnew("START");
	if (!newtoken)
	{
		free_token_list(*token);
		free_exit(core);
		exit(1);
	}
	ft_lstadd_back(token, newtoken);
	newtoken->type = 9999;
	newtoken->leading_space = 20;
}

void	make_end_token(t_token **token, t_data *core)
{
	t_token	*newtoken;

	newtoken = ft_lstnew("END");
	if (!newtoken)
	{
		free_token_list(*token);
		free_exit(core);
		exit(1);
	}
	ft_lstadd_back(token, newtoken);
	newtoken->type = 9999;
	newtoken->leading_space = 20;
}

void	substitute_redir(t_token *curr, char str[3],
	t_token *token, t_data *core)
{
	t_token	*newnext;
	t_token	*discard;

	newnext = curr->next->next;
	if (curr->next)
	{
		free(curr->next->word);
		discard = curr->next;
		curr->next = newnext;
		free(discard);
	}
	curr->word = malloc(3);
	if (curr->word == NULL)
	{
		free_token_list(token);
		free_exit(core);
		exit(1);
	}
	ft_strlcpy(curr->word, str, 3);
	if (newnext)
		newnext->prev = curr;
	curr->type = curr->type * 10;
}

int	whichtoken(char c)
{
	if (c == '<')
		return (1);
	else if (c == '>')
		return (2);
	else if (c == '|')
		return (3);
	else if (c == '"')
		return (4);
	else if (c == '\'')
		return (5);
	else if (c == '-')
		return (6);
	else if (c == '=')
		return (7);
	else if (c == '$')
		return (8);
	else
		return (0);
}
