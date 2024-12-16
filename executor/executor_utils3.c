/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 10:39:09 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/16 14:29:10 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fd_exit(int fd, t_cmdtable *cmd, t_data *core)
{
	error_handler_fd(fd, cmd);
	core->exit_status = 1;
	free_exit(core);
	exit(core->exit_status);
}

int	here_doc_counter(t_cmdtable *cmd)
{
	int	total;

	total = 0;
	while (cmd)
	{
		if (cmd->redir_type == 10 || cmd->redir_type == 30)
			total++;
		cmd = cmd->next;
	}
	return (total);
}

void	execution_pro(t_cmdtable *cmd, t_data *core, t_var *vars, int fd[2])
{
	if (cmd->redir_type != 0)
		redirctions(cmd, core, vars, fd);
	if (cmd->isbuiltin == 1)
		echo_cmd(cmd, core, 0);
	else if (cmd->args && ft_strchr(cmd->args[0], '/'))
		absolute_path_finder(core, core->env, cmd->args);
	else
		path_finder(vars, core, core->env, cmd->args);
	free_exit(core);
	exit(core->exit_status);
}

void	sig_int_parent3(int signal)
{
	(void)signal;
}

void	sig_int_parent2(int signal)
{
	write (1, "\n", 1);
	(void)signal;
}
