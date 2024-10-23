/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/10/23 14:49:20 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_redir(t_token	*token)
{
	t_token	*curr;

	curr = token;
	if (curr->type == 1 ||curr->type == 2 ||curr->type == 10 ||curr->type == 20)
		return (1);
	return(0);
}

int is_END(t_token *curr)
{
	if((strncmp(curr->word, "END", 3) == 0 && curr->type == 9999)) 
		return(1);
	return (0);
}

int is_START(t_token *curr)
{
	if((strncmp(curr->word, "START", 5) == 0 && curr->type == 9999)) 
		return(1);
	return (0);
}

int is_PIPE(t_token *curr)
{
	if((strncmp(curr->word, "|", 1) == 0 && curr->type == 3)) 
		return(1);
	return (0);
}

int get_strnum(t_token *curr)
{
	int i;

	i = 0;
	while((!(is_redir(curr)) && strncmp(curr->word,"|",1) != 0 && curr->type != 3) && !is_END(curr))
	{
		i++;
		curr = curr->next;
	}
	printf("STRNUM:%d",i);
	return(i);
}

t_token *get_args(t_cmdtable *cmd, t_token *token)
{
	t_token	*curr;
	int		i;
	int		strnum;

	i = 0;
	curr = token;
	if(is_START(curr))
		curr = curr->next;
	strnum = get_strnum(curr);
	cmd->args = malloc(sizeof(char*) *strnum);
	while(i < strnum)
	{
		cmd->args[i] = curr->word;
		printf("\nargs[%d]:%s\n",i,cmd->args[i]);
		i++;
		if (!(curr->next))
			break;
		curr = curr->next;
	}
		printf("\ncurr:%s\n",curr->word);
	return (curr);
}

void prep_nodes_for_exec(t_token *token)
{
	t_cmdtable	cmd;
	t_token		*curr;

	curr = token;
	
	if (is_redir(curr))
	{
		//handle redirection before cmd
	}
	else if (is_PIPE(curr))
	{

	}
	else
		get_args(&cmd,token);
}