/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:58:57 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/19 17:42:24 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern volatile sig_atomic_t	g_interrupt_received;

void	sig_int_parent(int signal)
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

void	sig_quit_child(int signal)
{
	if (signal == SIGQUIT)
	{
		(void) signal;
		rl_replace_line("", 0);
		rl_on_new_line();
		g_interrupt_received = signal;
	}
	if (signal == SIGINT)
	{
		(void) signal;
		rl_on_new_line();
		rl_redisplay();
		g_interrupt_received = signal;
	}
}

void	sig_int_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		g_interrupt_received = signal;
	}
}

void	setup_signal_handler(int signal, void (*handler)(int))
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
