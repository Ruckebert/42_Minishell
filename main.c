/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:58:57 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/19 14:09:28 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_state = -1;

void sig_handleINT(int signal)
{
	if(signal == SIGINT)
	{
		write(1, "PeePeeShell$ > ", 15);
		write(1,"\n",1);
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
			core.line = readline("PeePeeShell$ > ");
			//core.line = readline("");
			if (core.line == NULL)
			{
				if (isatty(STDIN_FILENO))
					write(1,"exit\n",5);
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
