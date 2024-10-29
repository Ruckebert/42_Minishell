/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:23 by marsenij          #+#    #+#             */
/*   Updated: 2024/10/29 15:00:03 by marsenij         ###   ########.fr       */
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

void	substitute_redir(t_token *curr,char str[3])
{
	t_token	*newnext;
	t_token	*discard;
	char	*newredir;

	newnext = curr->next->next;
	free(curr->next->word);
	discard = curr->next;
	curr->next = newnext;
	free(discard);
	newredir = malloc(3);
	curr->word = newredir;
	ft_strlcpy(curr->word, str, 3);
	if (newnext)
		newnext->prev = curr;
	curr->type = curr->type * 10;
}

void	combine_double_redirect(t_token	*token)
{
	t_token	*curr;

	curr = token;
	while (curr && curr->next)
	{
		if (!ft_strncmp(curr->word, "<\0",2)  && !ft_strncmp(curr->next->word, "<\0",2) && (curr->next->leading_space == 0))
			substitute_redir(curr,"<<\0");
		else if (!ft_strncmp(curr->word, ">\0",2)  && !ft_strncmp(curr->next->word, ">\0",2) && (curr->next->leading_space == 0))
			substitute_redir(curr,">>\0");
		curr = curr->next;
	}
}

void	combine_with_equal(t_token	*token)
{
	t_token	*curr;
	char	*temp;
	char	*res;
	
	curr = token;
	while (curr && curr->next && curr->next->next)
	{
		if (!ft_strcmp(curr->next->word, "="))
		{	
			if(curr->next->leading_space == 0)
			{
				res = ft_strjoin(curr->word,curr->next->word);
				if(curr->next->next->leading_space == 0)
				{
					curr = curr->next;
					ft_lstdelone(curr->prev);
					temp = res;
					res = ft_strjoin(temp,curr->next->word);
					free(curr->word);
					curr->word = res;
					ft_lstdelone(curr->next);
					free(temp);
				}
			}
		}
		curr = curr->next;
	}
}

t_token	*tokenize(t_data *core)
{
	t_token	*token;
	t_token *newtoken;
	char	*word;
	int		pos;
	int		oldpos;

	pos = 0;
	token = NULL;

	if (core->line[0] == '\0')
		return (NULL);
		
	newtoken = ft_lstnew("START");
	ft_lstadd_back(&token, newtoken);
	newtoken->type = 9999;
	newtoken->leading_space = 20;

	while (core->line[pos] != '\0')
	{
		while (is_myspace(&core->line[pos]))
			pos++;
		if(core->line[pos] == '\0')
			break;
		oldpos = pos;
		if (!(issep(&core->line[pos])) && !(isquote(&core->line[pos])))
			word = getword(&pos, &oldpos, core, token);
		else if (issep(&core->line[pos]))
			word = getsep(&pos, core);
		else if (isquote(&core->line[pos]))
			word = getquote(&pos, &oldpos, core, token);
		newtoken = ft_lstnew(word);
		if(token->next != NULL && core->line[oldpos - 1] == ' ')
			newtoken->leading_space = 1;
		else
			newtoken->leading_space = 0;			
		ft_lstadd_back(&token, newtoken);
		newtoken->type = whichtoken(core->line[oldpos]);
	}

	newtoken = ft_lstnew("END");
	ft_lstadd_back(&token, newtoken);
	newtoken->type = 9999;
	newtoken->leading_space = 20;

	combine_double_redirect(token);
	combine_with_equal(token);
	//printlist(token);
	return(token);
}

