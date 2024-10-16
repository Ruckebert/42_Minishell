/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:23 by marsenij          #+#    #+#             */
/*   Updated: 2024/10/16 15:51:53 by marsenij         ###   ########.fr       */
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
	else if (c == '=')
		return 7;
	else if (c == '$')
		return 8;
	else
		return (0);
}

char	*getword(int *pos, int *oldpos, t_data *core, t_token *token)
{
	char	*word;
	
	*pos += searchsep(&core->line[*oldpos]);
	word = malloc (*pos - *oldpos + 1);
	if(word == NULL)
	{
		perror("in getquote");
		free_token_list(token);
		//need to free everything from core too TODO: make a universal function that will free core in our whole code so that we dont have multiple ways to do that 
		exit (EXIT_FAILURE);
	}
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
//TODO: =word, >>, << tokens not working, need to iterate over list and fix it after the whole tokenizing process
char	*getquote(int *pos, int *oldpos, t_data *core, t_token *token)
{
	char	*word;
	
	*pos += searchquote(&core->line[*pos]);
	word = malloc (*pos - *oldpos + 1);
	if(word == NULL)
	{
		perror("in getquote");
		free_token_list(token);
		exit (EXIT_FAILURE);
		//need to free everything from core too TODO: make a universal function that will free core in our whole code so that we dont have multiple ways to do that 
	}
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

	pos = 0;
	token = NULL;
	while (core->line[pos] != '\0')
	{
		while (is_myspace(&core->line[pos]))
			pos++;
		oldpos = pos;
		if (!(issep(&core->line[pos])) && !(isquote(&core->line[pos])))
			word = getword(&pos, &oldpos, core, token);
		else if (issep(&core->line[pos]))
			word = getsep(&pos, core);
		else if (isquote(&core->line[pos]))
			word = getquote(&pos, &oldpos, core, token);
		newtoken = ft_lstnew(word);
		if(core->line[oldpos - 1] == ' ')
			newtoken->leading_space = 1;
		ft_lstadd_back(&token, newtoken);
		newtoken->type = whichtoken(core->line[oldpos]);
	}
	printlist(token);
	free_token_list(token);
}

