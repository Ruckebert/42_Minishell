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
	strlcpy(curr->word, str, 3);
	curr->type = curr->type * 10;
}

void	combine_double_redirect(t_token	*token)
{
	t_token	*curr;

	curr = token;
	while (curr && curr->next)
	{
		if (!ft_strncmp(curr->word, "<\0",2)  && !ft_strncmp(curr->next->word, "<\0",2))
			substitute_redir(curr,"<<\0");
		else if (!ft_strncmp(curr->word, ">\0",2)  && !ft_strncmp(curr->next->word, ">\0",2))
			substitute_redir(curr,">>\0");
		curr = curr->next;
	}
}

void	open_doublequote(t_token *curr)
{
	t_token	*newnext;
	t_token	*discard;
	char	*fusedword;
	char	*temp1;
	char	*temp2;

	temp2 = NULL;
	newnext = curr->next->next;
	discard = curr->next;
	if (curr->next->leading_space == 0)
	{
		temp2 = malloc (ft_strlen(curr->next->word));
		ft_strlcpy(temp2, &(curr->next->word[1]), ft_strlen(&(curr->next->word[1])));
		fusedword = ft_strjoin(curr->word, temp2);
		free(temp2);
		
	}
	if (curr->next->leading_space == 1)
	{
		temp1 = ft_strjoin(curr->word, " ");
		temp2 = malloc(ft_strlen(curr->next->word) - 1);
		ft_strlcpy(temp2, &(curr->next->word[1]), ft_strlen(&(curr->next->word[1])));
		fusedword = ft_strjoin(temp1, temp2);
		free(temp1);
		free(temp2);
	}
	free(curr->word);
	curr->word = fusedword;
	free(curr->next->word);
	free(discard);
	curr->next = newnext;
}

void	handle_doublequote(t_token *token)
{
	t_token	*curr;

	curr = token;
	while (curr && curr->next)
	{
		if (curr->next->type == 4)
		{
			open_doublequote(curr);	
		}
		curr = curr->next;	
	}
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
		else
			newtoken->leading_space = 0;
		ft_lstadd_back(&token, newtoken);
		newtoken->type = whichtoken(core->line[oldpos]);
	}
	combine_double_redirect(token);
	printlist(token);
	handle_doublequote(token);
	printlist(token);
	free_token_list(token);
}

