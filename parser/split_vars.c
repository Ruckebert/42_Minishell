/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/12 15:10:57 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	has_var_sep(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_isalnum(str[i]) && (str[i] != '_' ))
			return (i);
		i++;
	}
	return (0);
}

char	which_var_sep(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_isalnum(str[i]))
			return (str[i]);
		i++;
	}
	return ('\0');
}

static void	split_word(t_token *curr, int sep_index)
{
	char	*str1;
	char	*str2;
	t_token	*newtoken;

	str1 = malloc(sep_index + 1);
	str2 = malloc(ft_strlen(&curr->next->word[sep_index]) + 1);
	ft_strlcpy(str1, curr->next->word, sep_index + 1);
	ft_strlcpy(str2, &curr->next->word[sep_index],
		ft_strlen(&curr->next->word[sep_index]) + 1);
	free(curr->next->word);
	curr->next->word = str1;
	newtoken = ft_lstnew(str2);
	free(str2);
	ft_lstadd_next(&curr->next, newtoken);
}

void	split_vars_by_sep(t_token *token)
{
	t_token	*curr;
	int		sep_index;

	curr = token;
	while (curr)
	{
		if (curr->type == 8 && has_var_sep(curr->next->word)
			&& ft_strlen(curr->next->word) != 1)
		{
			sep_index = has_var_sep(curr->next->word);
			split_word(curr, sep_index);
		}
		curr = curr->next;
	}
}
