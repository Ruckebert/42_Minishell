/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokenparts.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:23 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/15 12:24:13 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*getword(int *pos, int *oldpos, t_data *core, t_token *token)
{
	char	*word;

	*pos += searchsep(&core->line[*oldpos]);
	word = malloc (*pos - *oldpos + 1);
	if (word == NULL)
	{
		free_token_list(token);
		free_exit(core);
		exit (EXIT_FAILURE);
	}
	word[*pos - *oldpos] = '\0';
	ft_strlcpy(word, &(core->line[*oldpos]), *pos - *oldpos + 1);
	return (word);
}

char	*getsep(int *pos, t_data *core, t_token *token)
{
	char	*word;

	word = malloc(2);
	if (word == NULL)
	{
		free_token_list(token);
		free_exit(core);
		exit (EXIT_FAILURE);
	}
	word[0] = core->line[*pos];
	word[1] = '\0';
	(*pos)++;
	return (word);
}

char	*getquote(int *pos, int *oldpos, t_data *core, t_token *token)
{
	char	*word;

	*pos += searchquote(&core->line[*pos]);
	word = malloc (*pos - *oldpos + 1);
	if (word == NULL)
	{
		free_token_list(token);
		free_exit(core);
		exit (EXIT_FAILURE);
	}
	word[*pos - *oldpos] = '\0';
	ft_strlcpy(word, &(core->line[*oldpos]), *pos - *oldpos + 1);
	return (word);
}
