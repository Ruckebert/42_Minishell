/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:58:57 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/15 11:26:46 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_interrupt_received = 0;

void	sig_handleINT_parent(int signal)
{
	if (signal == SIGINT)
	{
		write (1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_interrupt_received = signal;
	}
}

void	sig_handleINT_child(int signal)
{
	if (signal == SIGINT)
	{
		(void)signal;
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sig_handleINT_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		g_interrupt_received = signal;
	}
}

void sig_handleINT_parent2(int signal)
{
	write (1, "\n", 1);
	(void)signal;
}

void setup_signal_handler(int signal, void (*handler)(int)) 
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(signal, &sa, NULL) == -1)
	{
		perror("sigaction");
		exit(1);
	}
}

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
	if (g_interrupt_received != 0)
	{
		core->exit_status = 130;
		g_interrupt_received = 0;
	}
	core->line = readline("PeePeeShell$ > ");
	if (core->line == NULL)
		main_exit(core);
	add_history(core->line);
	token = tokenize(core);
	if (token)
	{
		core->cmd = parse(core, token);
		if (core->cmd)
			executor(core->cmd, core);
	}
	setup_signal_handler(SIGINT, sig_handleINT_parent);
	free(core->line);
	if (status >= 0)
		exit(core->exit_status);
}

int	main(int argc, char *argv[], char **env)
{
	t_data	core;
	t_token	*token;
	int		status;

	setup_signal_handler(SIGINT, sig_handleINT_parent);
	status = -1;
	core = (t_data){0};
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
