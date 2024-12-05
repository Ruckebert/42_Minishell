/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/05 14:54:38 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_redir(t_token	*token)
{
	t_token	*curr;

	curr = token;
	if (curr->type == 1 || curr->type == 2 || curr->type == 10 || curr->type == 20 || curr->type == 30)
		return (1);
	return(0);
}

int is_END(t_token *curr)
{
	if(strncmp(curr->word, "END", 3) == 0 && curr->type == 9999) 
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
	elem->args = NULL;
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
	while(curr->type != 3 && !is_END(curr))
	{
		if(is_redir(curr))
		{
			if(is_END(curr->next->next))
				break ;
			curr = curr->next->next;
			continue ;
		}
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
	
	if (cmd->args)
		return (token->next);
	i = 0;
	curr = token;

	strnum = get_strnum(curr);
	cmd->args = malloc(sizeof(char*) * (strnum + 1));
	while(i < strnum)
	{
		if(!is_redir(curr) &&!is_redir(curr->prev))
		{
			cmd->args[i] = curr->word;
			i++;
		}
		if (!(curr->next))
			break;
		curr = curr->next;
	}
	cmd->args[i] = NULL;
	cmd->has_pipe_after = 0;
	return (token);
}

t_token *add_redir(t_cmdtable *cmd,t_token *curr)
{
	
	cmd->redir_type = curr->type;
	if (curr->next->type == 30)
		cmd->redir_type = 30;
	cmd->redir = curr->next->word;
	curr = curr->next->next;
	return (curr);
}

void set_builtin_status(t_cmdtable *cmd)
{
	if (cmd->args && cmd->args[0] != NULL)
	{
		if (!ft_strcmp(cmd->args[0], "echo"))
			cmd->isbuiltin = 1;
		else if (!ft_strcmp(cmd->args[0], "cd"))
			cmd->isbuiltin = 2;
		else if (!ft_strcmp(cmd->args[0], "pwd"))
			cmd->isbuiltin = 3;
		else if (!ft_strcmp(cmd->args[0], "export"))
			cmd->isbuiltin = 4;
		else if (!ft_strcmp(cmd->args[0], "unset"))
			cmd->isbuiltin = 5;
		else if (!ft_strcmp(cmd->args[0], "env"))
			cmd->isbuiltin = 6;
		else if (!ft_strcmp(cmd->args[0], "exit"))
			cmd->isbuiltin = 7;
		else
			cmd->isbuiltin = 0;
	}
	else
	{
	    cmd->isbuiltin = 0;
	}
}

void find_builtins(t_cmdtable *cmd)
{
	t_cmdtable *curr;

	curr = cmd;
	while (curr != NULL)
	{
		set_builtin_status(curr);
		curr = curr->next;
	}
}

void copy_args(t_cmdtable *cmd)
{
	int strnum;

	strnum = 0;
	if(cmd->prev->args)
	{
		while(cmd->prev->args[strnum])
			strnum++;
		cmd->args = malloc(sizeof(char*) * (strnum + 1));
		strnum = 0;
		while(cmd->prev->args[strnum])
		{
			cmd->args[strnum] = ft_strdup(cmd->prev->args[strnum]);
			strnum++;
		}
	}
}

t_cmdtable *initialize_cmd_table(t_token **curr)
{
	t_cmdtable *cmd;
	t_cmdtable *newcmd;

	cmd = NULL;
	newcmd = ft_lstnew_cmd(NULL, 0);
	ft_lstadd_back_cmd(&cmd, newcmd);
	if (is_START(*curr))
		*curr = (*curr)->next;
	return cmd;
}

t_token *process_redir(t_cmdtable **newcmd, t_cmdtable **cmd, t_token *curr)
{
	if ((*newcmd)->redir)
	{
		*newcmd = ft_lstnew_cmd(NULL, 0);
		ft_lstadd_back_cmd(cmd, *newcmd);
		copy_args(*newcmd);
	}
	return add_redir(*newcmd, curr);
}

t_token *process_pipe(t_cmdtable **newcmd, t_cmdtable **cmd, t_token *curr)
{
	*newcmd = ft_lstnew_cmd(NULL, 0);
	ft_lstadd_back_cmd(cmd, *newcmd);
	(*newcmd)->prev->has_pipe_after = 1;
	return curr->next;
}

void process_tokens(t_cmdtable **cmd, t_token *curr)
{
	t_cmdtable *newcmd = *cmd;

	while (!is_END(curr))
	{
		if (is_redir(curr))
			curr = process_redir(&newcmd, cmd, curr);
		else if (is_PIPE(curr))
			curr = process_pipe(&newcmd, cmd, curr);
		else
			curr = get_args(newcmd, curr);
	}
}

t_cmdtable *prep_nodes_for_exec(t_token *token)
{
	t_cmdtable	*cmd;
	t_token		*curr;

	curr = token;
	cmd = initialize_cmd_table(&curr);
	process_tokens(&cmd, curr);
	find_builtins(cmd);
//    printlist(token);
	free_token_list(token);
//    print_cmdtable(cmd);
	return cmd;
}
