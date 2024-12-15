/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:24:10 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/15 12:32:35 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_exit_status(t_token *curr, t_data *core)
{
	ft_lstdelone(curr->next);
	free(curr->word);
	curr->word = ft_itoa(core->exit_status);
}

static void	handle_expandable_var(t_token **curr, char **env)
{
	char	*value;
	t_token	*tmp;

	tmp = (*curr)->prev;
	value = get_env_var((*curr)->next->word, env);
	if (value[0] == ' ' || value[0] == '\t')
		(*curr)->leading_space = 1;
	if ((ft_strlen(value) > 0) && (value[ft_strlen(value) - 1] == ' '
			|| value[ft_strlen(value) - 1] == '\t'))
		(*curr)->next->next->leading_space = 1;
	substitute_node_word(*curr, value);
	free(value);
	if (ft_strchr((*curr)->word, ' ') != NULL)
		split_to_token(*curr);
	*curr = tmp;
}

t_token	*handle_non_expandable_var(t_token *curr)
{
	if (curr->next->next->type != 9999)
		curr->next->next->leading_space = curr->leading_space;
	ft_lstdelone(curr->next);
	curr = curr->prev;
	ft_lstdelone(curr->next);
	return (curr);
}

void	expand_var(t_token *token, char **env, t_data *core)
{
	t_token	*curr;

	curr = token;
	while (curr && curr->next)
	{
		if (curr->type == 8 && curr->next->leading_space == 0
			&& (ft_isalpha(curr->next->word[0])
				|| !ft_strcmp(curr->next->word, "_")
				|| !ft_strcmp(curr->next->word, "?")))
		{
			if (!ft_strcmp(curr->next->word, "?"))
				handle_exit_status(curr, core);
			else if (is_expandable(curr->next->word, env))
				handle_expandable_var(&curr, env);
			else
				curr = handle_non_expandable_var(curr);
		}
		curr = curr->next;
	}
}

void	substitute_node_word(t_token *curr, char *new_word)
{
	t_token	*next_node;
	t_token	*discard;

	if (!curr || !curr->next)
		return ;
	next_node = curr->next->next;
	if (next_node)
		next_node->prev = curr;
	discard = curr->next;
	curr->next = next_node;
	free(discard->word);
	free(discard);
	free(curr->word);
	curr->word = ft_strdup(new_word);
}
