/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_in_doublequote.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:24:10 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/15 13:16:38 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*extract_var_name(char *temp, t_token *curr)
{
	if (*(temp + 1) == '?')
		return (ft_strdup("?"));
	return (parse_var_name(curr));
}

void	free_double_array(char **array)
{
	int	i;

	i = 0;
	if (array == NULL)
		return ;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free (array);
}

t_token	*handle_variable_expansion(t_token *curr, char **env,
	t_data *core, char *temp)
{
	char	*var;

	while (temp != NULL)
	{
		if (*(temp + 1) != ' ' && (*(temp + 1) == '_'
				|| ft_isalpha(*(temp + 1)) || *(temp + 1) == '?'))
		{
			var = extract_var_name(temp, curr);
			if (curr->freethis == NULL && is_expandable(var, env))
			{
				curr->freethis = malloc(2 * sizeof(char *));
				curr->freethis[0] = curr->word;
				curr->freethis[1] = NULL;
				curr->freethis_num = 1;
			}
			parsearound_var(curr, env, var, core);
		}
		if (curr->endloop == 1)
			break ;
		temp = ft_strchr(temp + 1, '$');
	}
	return (curr);
}

t_token	*expand_variables(t_token *curr, char **env, t_data *core)
{
	t_token	*tmp;
	char	*temp;

	tmp = curr->prev;
	temp = ft_strchr(curr->word, '$');
	curr = handle_variable_expansion(curr, env, core, temp);
	free_double_array(curr->freethis);
	if (curr->endloop == 1)
	{
		ft_lstdelone(curr);
		return (tmp);
	}
	return (curr);
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
			curr = expand_variables(curr, env, core);
		}
		curr = curr->next;
	}
}
