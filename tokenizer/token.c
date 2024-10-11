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

t_token	*ft_lstnew(char *word)
{
	t_token	*elem;

	elem = malloc(sizeof(t_token));
	if (!elem)
		return (NULL);
	elem->word = word;
	elem->next = NULL;
	return (elem);
}

t_token	*ft_lstlast(t_token *lst)
{
	t_token	*temp;

	if (!lst)
		return (NULL);
	temp = lst;
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

void	ft_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*end;

	if (!new)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		end = ft_lstlast(*lst);
		end->next = new;
	}
}

#include <stdio.h>

int whichtoken(char *word)
{
	if (word[1] != '\0') 
		return 0;
	if (word[0] == '<')
		return 1;
	else if (word[0] == '>')
		return 2;
	else if (word[0] == '|')
		return 3;
	else if (word[0] == '"')
		return 4;
	else if (word[0] == '\'')
		return 5; 
	else 
		return (1919);
}

int isquote(char *c)
{
	if (*c == '\'')
		return (2);
	if (*c == '"')
		return (1);
	return (0);
}

int issep(char *c)
{
	if ((*c == '<' && *(c + 1) == '<')|| (*c == '>' && *(c + 1) == '>') )
		return (2);
	if (*c == '<' || *c == '>' ||  *c == '|')
		return (1);
	return (0);
}

int searchquote(char *str)
{
	int i;
	char quote_type = *str;

	i = 1;
	while (str[i] != '\0' && str[i] != quote_type)
		i++;
	if (str[i] == quote_type)
		i++;  // Include the closing quote
	return (i);
}

int searchsep(char *str)
{
	int i;

	i = 0;
	while(!(issep(&str[i])) && str[i] != '\0')
	{
		if(isquote(&str[i]))
			return(i);
		i++;
	}
	return (i);
}

// void addsep(t_data *core, t_token *token)
// {
	
// }


void tokenize(t_data *core)
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
	oldpos = 0;
	pos = 0;
	token = NULL;
	while (core->line[pos] != '\0')
	{
		oldpos = pos;
		if (!(issep(&core->line[pos])) && !(isquote(&core->line[pos])))
		{
			pos += searchsep(&core->line[oldpos]);
			word = malloc (pos - oldpos + 1);
			if(word == NULL)
				perror("Malloc failed");
			word[pos - oldpos] = '\0';
			ft_strlcpy(word, &(core->line[oldpos]), pos - oldpos + 1);
		}
		else if (issep(&core->line[pos]))
		{
			word = malloc(2);
			word[0] = core->line[pos];
			word[1] = '\0';
			pos++;
		}
		else if (isquote(&core->line[pos]))
		{
			pos += searchquote(&core->line[pos]);
			word = malloc (pos - oldpos + 1);
			if(word == NULL)
				perror("Malloc failed");
			word[pos - oldpos] = '\0';
			ft_strlcpy(word, &(core->line[oldpos]), pos - oldpos + 1);
		}
		newtoken = ft_lstnew(word);
		ft_lstadd_back(&token, newtoken);
		newtoken->type = whichtoken(&core->line[pos]);
	}
	printlist(token);
}
