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

int whichsep(char *word)
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

int issep(char *c)
{
	if ((*c == '<' && *(c + 1) == '<')|| (*c == '>' && *(c + 1) == '>') )
		return (2);
	if (*c == '<' || *c == '>' ||  *c == '|')
		return (1);
	return (0);
}

int searchsep(char *str)
{
	int i;

	i = 0;
	while(!(issep(&str[i])) && str[i] != '\0')
		i++;
	return (i);
}

// void addsep(t_data *core, t_token *token)
// {
	
// }

// void tokenize(t_data *core)
// {
// 	t_token	*token;
// 	t_token *newtoken;
// 	char	*word;
// 	int		pos;
// 	int		oldpos;

// 	oldpos = 0;
// 	pos = 0;
// 	while (core->line[oldpos] != '\0')
// 	{
// 		if (issep(&core->line[pos]))
// 		{
// 			word = malloc(2);
// 			word[0] = core->line[pos];
// 			word[1] = '\0';
// 			newtoken = ft_lstnew(word);
// 			ft_lstadd_back(&token, newtoken);
// 			newtoken->type = whichsep(&core->line[pos]);
// 			pos++;
// 		}
// 		else
// 		{
// 			pos = searchsep(&core->line[oldpos]);
// 			word = malloc (pos - oldpos);
// 			ft_strlcpy(word, &(core->line[oldpos]), pos);
// 			newtoken = ft_lstnew(word);
// 			ft_lstadd_back(&token, newtoken);
// 			newtoken->type = whichsep(&core->line[pos]);
// 			oldpos = pos;
// 		}
// 		printlist(token);
// 	}
// }

// int	main(int argc, char **argv)
// {
// 	t_token	*token;
// 	char	**nopipe;
// 	char	**words;
// 	int		i;
// 	int		j;
// 	t_token *newtoken;
	
// 	i = 0;
// 	j = 0;
// 	nopipe = NULL;
// 	words = NULL;
// 	token = NULL;
// 	nopipe = ft_split(argv[1], '|');
// 	while (nopipe[i])
// 	{
// 		printf("NOPIPE ARRAY:%s\n\n",nopipe[i]);	
// 		words = ft_split(nopipe[i], ' ');
// 		while (words[j])
// 		{
// 			printf("WORDS ARRAY:%s\n\n",words[j]);	
// 			newtoken = ft_lstnew(words[j]);
// 			ft_lstadd_back(&token, newtoken);
// 			j++;
// 			newtoken->type = add_tokentype(newtoken->word);
// 		}
// 		//free words
// 		if(nopipe[i+1] != NULL)
// 		{
// 			newtoken = ft_lstnew("|");
// 			ft_lstadd_back(&token, newtoken);
// 			newtoken->type = 9;
// 		}
// 		j = 0;
// 		i++;
// 	}
// 	//free nopipe
// 	printlist(token);
// 	printlist_type(token);
// //printCharPointerArray(res);
// }