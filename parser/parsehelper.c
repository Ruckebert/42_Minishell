/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsehelper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:24:10 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/19 12:40:45 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_expandable(char *var, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(var, env[i], ft_strlen(var))
			&& env[i][ft_strlen(var)] == '=')
		{
			return (1);
		}
		i++;
	}
	return (0);
}

void	remove_quotes(t_token *curr)
{
	char	*temp;
	t_data	*core;

	if (curr->word[0] == '\'' || curr->word[0] == '\"')
	{
		temp = malloc(ft_strlen(curr->word) - 1);
		if (!temp)
		{
			core = address_getter(NULL);
			free_token_list(curr);
			free_exit(core);
			exit(1);
		}
		ft_strlcpy(temp, &curr->word[1], ft_strlen(curr->word) - 1);
		free(curr->word);
		curr->word = temp;
	}
}

int	is_heredoc(t_token *token)
{
	t_token	*curr;

	curr = token;
	if (curr->type == 10)
		return (1);
	return (0);
}

void	remove_singlequotes(t_token *token)
{
	t_token	*curr;

	curr = token;
	while (curr)
	{
		if (curr->type == 5)
		{
			remove_quotes(curr);
		}
		curr = curr->next;
	}
}

int	is_fusable(t_token *curr)
{
	if (curr->type == 8 || curr->type == 4
		|| curr->type == 5 || curr->type == 7
		|| curr->type == 9 || curr->type == 70)
		return (1);
	else
		return (0);
}
