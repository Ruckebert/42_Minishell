/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/10/24 14:49:46 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_redir(t_token	*token)
{
	t_token	*curr;

	curr = token;
	if (curr->type == 1 || curr->type == 2 || curr->type == 10 || curr->type == 20)
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

t_cmdtable	*ft_lstnew_cmd(char *redir, int type)
{
	t_cmdtable	*elem;

	elem = malloc(sizeof(t_cmdtable));
	if (!elem)
		return (NULL);
	elem->redir = redir;
	elem->redir_type = type;
	elem->next = NULL;
	elem->prev = NULL;
	return (elem);
}

t_cmdtable	*ft_lstlast_cmd(t_cmdtable *lst)
{
	t_cmdtable	*temp;

	if (!lst)
		return (NULL);
	temp = lst;
	while (temp->next != NULL)
		temp = temp->next;
	return (temp);
}

void	ft_lstadd_back_cmd(t_cmdtable **lst, t_cmdtable *new)
{
	t_cmdtable	*end;

	if (!new)
		return;
	if (*lst == NULL)
	{
		*lst = new;
	}
	else
	{
		end = ft_lstlast_cmd(*lst);
		end->next = new;
		new->prev = end;
	}
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
	cmd->args = malloc(sizeof(char*) * (strnum + 1));
	while(i < strnum)
	{
		cmd->args[i] = curr->word;
		i++;
		if (!(curr->next))
			break;
		curr = curr->next;
	}
	return (curr);
}

t_token *add_redir(t_cmdtable *cmd,t_token *curr)
{
	if (curr->next->type != 0)
	{
		ft_printf("no redir name after redir symbol");
		exit(7);
	}
	else
	{
		cmd->redir_type = curr->type;
		cmd->redir = curr->next->word;
		curr = curr->next->next;
	}
	return (curr);
}

void find_builtins(t_cmdtable *cmd)
{
	t_cmdtable *curr;

	curr = cmd;
	while(curr != NULL)
	{
		if (!ft_strcmp(curr->args[0], "echo"))
			curr->isbuiltin = 1;
		else if (!ft_strcmp(curr->args[0], "cd"))
			curr->isbuiltin = 2;
		else if (!ft_strcmp(curr->args[0], "pwd"))
			curr->isbuiltin = 3;
		else if (!ft_strcmp(curr->args[0], "export"))
			curr->isbuiltin = 4;
		else if (!ft_strcmp(curr->args[0], "unset"))
			curr->isbuiltin = 5;
		else if (!ft_strcmp(curr->args[0], "env"))
			curr->isbuiltin = 6;
		else if (!ft_strcmp(curr->args[0], "exit"))
			curr->isbuiltin = 7;
		else
			curr->isbuiltin = 0;
		curr = curr->next;
	}
}

void prep_nodes_for_exec(t_token *token)
{
	t_cmdtable	*cmd;
	t_token		*curr;
	t_cmdtable	*newcmd;

	curr = token;
	cmd = NULL;
	newcmd = ft_lstnew_cmd(NULL, 0);
	ft_lstadd_back_cmd(&cmd, newcmd);
	while (!is_END(curr))
	{
		if (is_redir(curr))
		{
			curr = add_redir(newcmd, curr);
		}
		else if (is_PIPE(curr))
		{
			newcmd = ft_lstnew_cmd(NULL, 0);
			ft_lstadd_back_cmd(&cmd, newcmd);
			newcmd->prev->has_pipe_after = 1;
			curr = curr->next;
		}
		else
			curr = get_args(newcmd,curr);
	}
	find_builtins(cmd);
	free_token_list(token);
//	print_cmdtable(cmd);
//	free_cmdtable(&cmd);
}
