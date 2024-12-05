/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:58:57 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/05 11:19:31 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_interrupt_received = 0;

void sig_handleINT(int signal)
{
	if(signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();	
		g_interrupt_received = 128 + signal;
	}
}

int main(int argc, char *argv[], char **env)
{
	t_data core;
	t_token *token;

	int error = -999;
	struct sigaction SA_parent;

	SA_parent.sa_handler = sig_handleINT;
	SA_parent.sa_flags = 0;
	sigemptyset(&SA_parent.sa_mask);
	error = sigaction(SIGINT, &SA_parent, NULL);
	if (error == -1)
		exit (254);
		
	int status = -1;

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
	if (/*isatty(STDIN_FILENO) ==*/ 1 /*true*/) //The isatty is the reason why the tester doesnt work
	{
		while (status == -1)
		{
			if (g_interrupt_received != 0)
			{
				core.exit_status = 130;
				g_interrupt_received = 0;
			}
			core.line = readline("PeePeeShell$ > ");
			if (core.line == NULL)
			{
				if (isatty(STDIN_FILENO))
					write(2,"exit\n",5);
				exit (core.exit_status);
			}
			add_history(core.line);
			token = tokenize(&core);
			if(token)
			{
				core.cmd = parse(&core, token);
				if (core.cmd)
					executor(core.cmd, &core);
			}
			free(core.line);
			if (status >= 0)
				exit(core.exit_status);
		}
	}
	return (0);
}
