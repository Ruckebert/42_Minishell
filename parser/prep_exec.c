/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:43:26 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/17 16:55:26 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmdtable	*initialize_cmd_table(t_token **curr)
{
	t_cmdtable	*cmd;
	t_cmdtable	*newcmd;

	cmd = NULL;
	newcmd = ft_lstnew_cmd(NULL, 0);
	ft_lstadd_back_cmd(&cmd, newcmd);
	if (is_start(*curr))
		*curr = (*curr)->next;
	return (cmd);
}

t_token	*process_redir(t_cmdtable **newcmd, t_cmdtable **cmd, t_token *curr)
{
	if ((*newcmd)->redir)
	{
		*newcmd = ft_lstnew_cmd(NULL, 0);
		ft_lstadd_back_cmd(cmd, *newcmd);
		copy_args(*newcmd);
	}
	return (add_redir(*newcmd, curr));
}

t_token	*process_pipe(t_cmdtable **newcmd, t_cmdtable **cmd, t_token *curr)
{
	*newcmd = ft_lstnew_cmd(NULL, 0);
	ft_lstadd_back_cmd(cmd, *newcmd);
	(*newcmd)->prev->has_pipe_after = 1;
	return (curr->next);
}

void	*process_tokens(t_cmdtable **cmd, t_token *curr)
{
	t_cmdtable	*newcmd;

	newcmd = *cmd;
	while (!is_end(curr))
	{
		if (is_redir(curr))
			curr = process_redir(&newcmd, cmd, curr);
		else if (is_pipe(curr))
			curr = process_pipe(&newcmd, cmd, curr);
		else
			curr = get_args(newcmd, curr);
		if (curr == NULL)
			return (NULL);
	}
	return ((void *)1);
}

void print_cmdtable(t_cmdtable *cmd)
{
    int i;
    t_cmdtable *current = cmd;

    printf("-------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-10s | %-15s | %-10s | %-10s |\n", 
           "Node", "Arguments", "Builtin", "Redir Type", "Redir", "Pipe After");
    printf("-------------------------------------------------------------------------------------------\n");

    int node_num = 1;
    while (current != NULL)
    {
        // Print node number
        printf("| %-5d | ", node_num);

        // Print arguments
        if (current->args)
        {
            printf("[");
            for (i = 0; current->args[i] != NULL; i++)
            {
                if (current->args[i] == NULL )  // Safety check for NULL values
                {
                    printf("NULL");
                    break;
                }
                printf("%s", current->args[i]);
                if (current->args[i + 1] != NULL)
                    printf(", ");
            }
            printf("] ");
        }
        else
        {
            printf("No Args           ");
        }

        // Print whether it is a built-in command
        printf("| %-10d | ", current->isbuiltin);

        // Print redir_type and redir values
        printf("| %-15d | %-10s | ", current->redir_type, current->redir ? current->redir : "No Redir");

        // Print whether it has a pipe after
        printf("%-10s |\n", current->has_pipe_after ? "Yes" : "No");

        // Print divider line
        printf("-------------------------------------------------------------------------------------------\n");

        // Safety check before moving to the next node
        if (current->next == current)  // Prevent self-referencing nodes
        {
            printf("Error: Self-referencing node detected. Aborting to prevent infinite loop.\n");
            break;
        }

        current = current->next;
        node_num++;
    }
}



t_cmdtable	*prep_nodes_for_exec(t_token *token, t_data *core)
{
	t_cmdtable	*cmd;
	t_token		*curr;

//	cmd = (t_cmdtable *){0};
	if (!token)
		return (NULL);
	curr = token;
	cmd = initialize_cmd_table(&curr);
	if (process_tokens(&cmd, curr) == NULL)
	{
		free_token_list(token);
		free_exit(core);
		if (cmd->redir)
			free(cmd->redir);
		free(cmd);
		exit(core->exit_status);
	}
	find_builtins(cmd);
	if (token)
		free_token_list(token);		
	return (cmd);
}
