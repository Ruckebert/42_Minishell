/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:58:57 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/19 17:41:07 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_interrupt_received = 0;

int	main_exit(t_data *core)
{
	if (isatty(STDIN_FILENO))
		write (2, "exit\n", 5);
	if (core->env != NULL)
		simple_free(core->env);
	if (core->export_env != NULL)
		simple_free(core->export_env);
	free(core->direct);
	free(core->line);
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exit(core->exit_status);
}

void	main_core(t_data *core, t_token	*token, int status)
{
	core->empty_cd = 0;
	if (g_interrupt_received == 3)
		ft_putstr_fd("Quit (core dumped)\n", 2);
	core->line = readline("PeePeeShell$ > ");
	if (core->line == NULL)
		main_exit(core);
	if (g_interrupt_received != 0)
	{
		core->exit_status = 128 + g_interrupt_received;
		g_interrupt_received = 0;
	}
	add_history(core->line);
	token = tokenize(core);
	if (token)
	{
		core->cmd = parse(core, token);
		if (core->cmd)
			executor(core->cmd, core);
	}
	setup_signal_handler(SIGINT, sig_int_parent);
	setup_signal_handler(SIGQUIT, SIG_IGN);
	free(core->line);
	if (status >= 0)
		exit(core->exit_status);
}

int	main(int argc, char *argv[], char **env)
{
	t_data	core;
	t_token	*token;
	int		status;

	setup_signal_handler(SIGQUIT, SIG_IGN);
	setup_signal_handler(SIGINT, sig_int_parent);
	status = -1;
	core = (t_data){0};
	address_getter(&core);
	token = NULL;
	core.exit_status = 0;
	if (argc == -1)
		exit(2);
	core.env = copy_env(env, &core);
	if (core.env == NULL)
		return (2);
	(void)argc;
	(void)argv;
	core.export_env[0] = NULL;
	core.empty_cd = 0;
	while (status == -1)
		main_core(&core, token, status);
	return (0);
}
