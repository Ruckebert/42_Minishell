/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_in_doublequote.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:24:10 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/11 15:49:23 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*extract_var_name(char *temp, t_token *curr)
{
	if (*(temp + 1) == '?')
		return (ft_strdup("?"));
	return (parse_var_name(curr));
}

static void	expand_variables(t_token *curr, char **env, t_data *core)
{
	char	*temp;
	char	*var;

	temp = ft_strchr(curr->word, '$');
	while (temp != NULL)
	{
		if (*(temp + 1) != ' ' && (*(temp + 1) == '_'
				|| ft_isalpha(*(temp + 1)) || *(temp + 1) == '?'))
		{
			if (curr->freethis == NULL)
				curr->freethis = curr->word;
			var = extract_var_name(temp, curr);
			parsearound_var(curr, env, var, core);
		}
		temp = ft_strchr(temp + 1, '$');
	}
	//make curr->freethis a 2d array put a null right here and add every string that needs to be gone into that 2d array and free it here
	if (curr->freethis)//fix this
	{
		free(curr->freethis);
		curr->freethis = NULL;
	}
}

void	expand_var_in_doublequote(t_token *token, char **env, t_data *core)
{
	t_token	*curr;

	curr = token;
	while (curr)
	{
		if (curr->type == 4)
		{
			remove_quotes(curr);
			expand_variables(curr, env, core);
		}
		curr = curr->next;
	}
}