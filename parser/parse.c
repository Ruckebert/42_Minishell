/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:24:10 by marsenij          #+#    #+#             */
/*   Updated: 2024/10/18 13:02:18 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void substitute_node_word(t_token *curr, char *new_word)
{
	t_token *next_node;
	t_token *discard;
	
	if (!curr || !curr->next) 
	    return;
	next_node = curr->next->next;
	if (next_node)
		next_node->prev = curr;
	discard = curr->next;
	curr->next = next_node;
	free(discard->word);
	free(discard);
	free(curr->word);
	curr->word = strdup(new_word);
}

void fuse_node_word(t_token *curr, char *new_word)
{
	t_token *next_node;
	t_token *discard;
	char	*oldword;
	
	oldword = curr->word;
	if (!curr || !curr->next) 
	    return;
	next_node = curr->next->next;
	if (next_node)
		next_node->prev = curr;
	discard = curr->next;
	curr->next = next_node;
	free(discard->word);
	free(discard);
	curr->word = ft_strjoin(oldword,new_word);
	free(oldword);
}

char *get_env_var(char *var, char **env)
{
	int     i;
	char    *res;
	int     valuestart;

	i = 0;
	res = NULL;
	while (env[i])
	{
		if (!strncmp(var, env[i], ft_strlen(var)) && env[i][ft_strlen(var)] == '=')
		{
			valuestart = ft_strlen(var) + 1;
			res = malloc(ft_strlen(&env[i][valuestart]) + 1);
			if (!res) 
				return NULL;
			ft_strlcpy(res, &env[i][valuestart], ft_strlen(&env[i][valuestart]) + 1);
			return (res);
		}
		i++;
	}
	printf("Variable not found");//TODO:free and return properly
	return (NULL);
}


void expand_var(t_token *token, char **env)
{
	char	*value;
	t_token	*curr;
	
	curr = token;
	while (curr && curr->next)
	{
		if (curr->type == 8 && curr->next->leading_space == 0)//there is a case here where $sth doesnt exist so it should expand to nothing (i guess)
		{
			if(!strncmp(curr->next->word,"?\0", 2))
			{
					// handle $? token
			}
			else 
			{
				value = get_env_var(curr->next->word, env);
				substitute_node_word(curr, value);
				if (curr->leading_space == 0)
					fuse_node_word(curr->prev, value);
			}
				
		}
		curr = curr->next;
	}
}

void parse(t_data *core, char **env, t_token *token)
{
	(void) core;
	handle_doublequote(token);
//	printlist_both(token);
	expand_var(token, env);
//	printlist(token);
//	printlist(token);
//	handle_singlequote();
	
}