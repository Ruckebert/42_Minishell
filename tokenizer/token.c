/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:23 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/11 15:06:28 by marsenij         ###   ########.fr       */
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

void	make_tokens(t_data *core, t_token *token, int pos)
{
	t_token	*newtoken;
	char	*word;
	int		oldpos;

	while (core->line[pos] != '\0')
	{
		while (is_myspace(&core->line[pos]))
			pos++;
		if (core->line[pos] == '\0')
			break ;
		oldpos = pos;
		if (!(issep(&core->line[pos])) && !(isquote(&core->line[pos])))
			word = getword(&pos, &oldpos, core, token);
		else if (issep(&core->line[pos]))
			word = getsep(&pos, core, token);
		else if (isquote(&core->line[pos]))
			word = getquote(&pos, &oldpos, core, token);
		newtoken = ft_lstnew(word);
		free(word);
		if (!newtoken)
		{
			free_token_list(token);
			free_exit(core);
			exit(1);
		}
		if (token->next != NULL && core->line[oldpos - 1] == ' ')
			newtoken->leading_space = 1;
		else
			newtoken->leading_space = 0;
		ft_lstadd_back(&token, newtoken);
		newtoken->type = whichtoken(core->line[oldpos]);
	}
}

t_token	*tokenize(t_data *core)
{
	t_token	*token;
	int		pos;

	pos = 0;
	token = NULL;
//	printlist(token);
	make_start_token(&token, core);
	make_tokens(core, token, pos);
	printlist(token);
	make_end_token(&token, core);
	printlist(token);
	combine_double_redirect(token, core);
	printlist(token);
	remove_empty_quotes(token);

	if (synthax_check(token, core) != 0)
		return (NULL);
	return (token);
}
