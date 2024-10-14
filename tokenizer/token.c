/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:23 by marsenij          #+#    #+#             */
/*   Updated: 2024/10/11 15:16:24 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	whichtoken(char c)
{
	if (c == '<')
		return 1;
	else if (c == '>')
		return 2;
	else if (c == '|')
		return 3;
	else if (c == '"')
		return 4;
	else if (c == '\'')
		return 5;
	else if (c == '-')
		return 6;
	else 
		return (0);
}

char	*getword(int *pos, int *oldpos, t_data *core)
{
	char	*word;
	
	*pos += searchsep(&core->line[*oldpos]);
	word = malloc (*pos - *oldpos + 1);
	if(word == NULL)
		perror("Malloc failed");
	word[*pos - *oldpos] = '\0';
	ft_strlcpy(word, &(core->line[*oldpos]), *pos - *oldpos + 1);
	return (word);
}

char	*getsep(int *pos, t_data *core)
{
	char *word;

	word = malloc(2);
	word[0] = core->line[*pos];
	word[1] = '\0';
	(*pos)++;
	return (word);
}

char	*getquote(int *pos, int *oldpos, t_data *core)
{
	char	*word;
	
	*pos += searchquote(&core->line[*pos]);
	word = malloc (*pos - *oldpos + 1);
	if(word == NULL)
		perror("Malloc failed");
	word[*pos - *oldpos] = '\0';
	ft_strlcpy(word, &(core->line[*oldpos]), *pos - *oldpos + 1);
	return (word);
}

void	tokenize(t_data *core)
{
	t_token	*token;
	t_token *newtoken;
	char	*word;
	int		pos;
	int		oldpos;

	//Gameplan:
	//walk till you reach seperator or a quote
	//if quote go until next quote and copy everything until next quote
	//else if seperator copy it
	//else copy the token until quote seperator or space
	pos = 0;
	token = NULL;
	while (core->line[pos] != '\0')
	{
		while (is_myspace(&core->line[pos]))
			pos++;
		oldpos = pos;
		if (!(issep(&core->line[pos])) && !(isquote(&core->line[pos])))
			word = getword(&pos, &oldpos, core);
		else if (issep(&core->line[pos]))
			word = getsep(&pos, core);
		else if (isquote(&core->line[pos]))
			word = getquote(&pos, &oldpos, core);
		newtoken = ft_lstnew(word);
		ft_lstadd_back(&token, newtoken);
		newtoken->type = whichtoken(core->line[oldpos]);
	}
	printlist(token);
	free_token_list(token);

}

