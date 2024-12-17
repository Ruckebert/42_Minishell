/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:53:00 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/17 11:37:12 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_interrupt_received;

void	pipe_wait(int status, pid_t second, t_cmdtable *cmd, t_data *core)
{
	setup_signal_handler(SIGINT, sig_int_parent2);
	waitpid(second, &status, 0);
	if (WIFEXITED(status))
		core->exit_status = WEXITSTATUS(status);
	if (g_interrupt_received == 3 || g_interrupt_received == 2)
		g_interrupt_received = 0;
	free_cmdtable(&cmd);
}
