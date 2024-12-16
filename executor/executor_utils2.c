/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:56:10 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/16 11:03:33 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	here_doc_null_msg(t_cmdtable *cmd)
{
	ft_putstr_fd("warning: here-document delimited by end-of-file ", 2);
	ft_putstr_fd("(wanted `", 2);
	ft_putstr_fd(cmd->redir, 2);
	ft_putstr_fd("')\n", 2);
}

void	expander_freer(t_exp *doc)
{
	if (doc->var_name)
		free(doc->var_start);
	if (doc->expanded_line)
		free(doc->expanded_line);
	if (doc->env_value)
		free(doc->env_value);
	exit(1);
}

int	cmd_count(t_cmdtable *cmd)
{
	int	total;

	total = 0;
	while (cmd)
	{
		total++;
		if (cmd->has_pipe_after != 1)
		{
			while (cmd)
			{
				if (cmd->has_pipe_after == 1)
					break ;
				cmd = cmd->next;
			}
		}
		if (cmd == NULL)
			break ;
		cmd = cmd->next;
	}
	return (total);
}

int	pipe_checker(t_cmdtable *cmd)
{
	t_cmdtable	*tmp;
	int			i;

	i = 0;
	tmp = cmd;
	while (cmd)
	{
		if (cmd->has_pipe_after == 1)
			i++;
		cmd = cmd->next;
	}
	cmd = tmp;
	return (i);
}
