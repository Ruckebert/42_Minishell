/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:53:00 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/17 11:34:10 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pipe_wait(int status, pid_t second, t_cmdtable *cmd, t_data *core)
{
	setup_signal_handler(SIGINT, sig_int_parent2);
	waitpid(second, &status, 0);
	if (WIFEXITED(status))
		core->exit_status = WEXITSTATUS(status);
	free_cmdtable(&cmd);
}

void	type_close(int fd, t_data *core)
{
	if (close(fd) == -1)
		close_fail(core, 0);
}
