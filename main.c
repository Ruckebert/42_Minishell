/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:58:57 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/20 14:13:38 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t interrupt_received = 0;

void sig_handleINT(int signal)
{
	if(signal == SIGINT)
	{
		//ioctl(STDIN_FILENO, TIOCSTI, "\n");
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();	
		interrupt_received = 128 + signal;
	}
}

int main(int argc, char *argv[], char **env)
{
	t_data core;
	t_token *token;

	int error = -999;
	struct sigaction SI;

	
	SI.sa_handler = sig_handleINT;
	SI.sa_flags = 0;
	sigemptyset(&SI.sa_mask);
	error = sigaction(SIGINT, &SI, NULL);
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
			/*if (interrupt_received != 0)
			{
				core.exit_status = 130;
				interrupt_received = 0;
				continue;
			}*/
			core.line = readline("PeePeeShell$ > ");
			//core.line = readline("");
			if (core.line == NULL)
			{
				if (isatty(STDIN_FILENO))
					write(2,"exit\n",6);
				exit (core.exit_status);
			}
			add_history(core.line);
			token = tokenize(&core);
			if(token)
			{
				core.cmd = parse(&core, token);
				executor(core.cmd, &core);
			}
			free(core.line);
			//printf("Exit Status: %d\n", core.exit_status);
			if (status >= 0)
				exit(core.exit_status);
		}
	}
	return (0);
}
