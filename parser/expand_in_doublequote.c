/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_in_doublequote.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:24:10 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/18 18:08:19 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*extract_var_name(char *temp, t_token *curr)
{
	char	*tmp;

	if (*(temp + 1) == '?')
	{
		tmp = ft_strdup("?");
		if (!tmp)
			free_all();
		return (tmp);
	}
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
	free(array);
}

int	ft_only_squote(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '\'')
			return (0);
		i++;
	}
	return (1);
}

t_token	*expand_variables(t_token *curr, char **env,
	t_data *core)
{
	char	*temp;
	char	*var;
	t_token	*tmp;

	tmp = curr->prev;
	temp = ft_strchr(curr->word, '$');
	while (temp != NULL)
	{
		if (*(temp + 1) != ' ' && (*(temp + 1) == '_'
				|| ft_isalpha(*(temp + 1)) || *(temp + 1) == '?'))
		{
			var = extract_var_name(temp, curr);
			if (curr->freethis == NULL && is_expandable(var, env))
				init_freethis(curr);
			if (parsearound_var(curr, env, var, core) == -1)
				free_all();
		}
		if (curr->endloop == 1)
			break ;
		temp = ft_strchr(temp + 1, '$');
	}
	free_double_array(curr->freethis);
	if (curr->endloop == 1 && ft_strlen(curr->word) == 0)
		return (ft_lstdelone(curr), tmp);
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
