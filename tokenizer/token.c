/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:23 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/15 12:25:14 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	frexit_token(t_token *token, t_data *core)
{
	free_token_list(token);
	free_exit(core);
	exit(1);
}

void	process_token(t_data *core, t_token *token, int *pos)
{
	t_token		*newtoken;
	char		*word;
	int			oldpos;

	word = NULL;
	oldpos = *pos;
	if (!(issep(&core->line[*pos])) && !(isquote(&core->line[*pos])))
		word = getword(pos, &oldpos, core, token);
	else if (issep(&core->line[*pos]))
		word = getsep(pos, core, token);
	else if (isquote(&core->line[*pos]))
		word = getquote(pos, &oldpos, core, token);
	newtoken = ft_lstnew(word);
	free(word);
	if (!newtoken)
		frexit_token(token, core);
	if (token->next != NULL && core->line[oldpos - 1] == ' ')
		newtoken->leading_space = 1;
	else
		newtoken->leading_space = 0;
	ft_lstadd_back(&token, newtoken);
	newtoken->type = whichtoken(core->line[oldpos]);
}

void	skip_spaces(t_data *core, int *pos)
{
	while (is_myspace(&core->line[*pos]))
		(*pos)++;
}

void	make_tokens(t_data *core, t_token *token, int pos)
{
	while (core->line[pos] != '\0')
	{
		skip_spaces(core, &pos);
		if (core->line[pos] == '\0')
			break ;
		process_token(core, token, &pos);
	}
}

t_token	*tokenize(t_data *core)
{
	t_token	*token;
	int		pos;

	pos = 0;
	token = NULL;
	make_start_token(&token, core);
	make_tokens(core, token, pos);
	make_end_token(&token, core);
	combine_double_redirect(token, core);
	remove_empty_quotes(token);
	if (synthax_check(token, core) != 0)
		return (free_token_list(token), NULL);
	return (token);
}
