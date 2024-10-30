/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:58:57 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/30 16:27:24 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Simply a Test Function to test certain things for the executor
//It needs alot of work though :(
int main(int argc, char *argv[], char **env)
{
	t_data core;
	t_token *token;
	
	int status = -1;

	token = NULL;
	if (argc == -1)
		exit(2);
	core.env = copy_env(env, &core);
	if (core.env == NULL)
		return (2);
	(void)argc;
	(void)argv;
	core.export_env[0] = NULL;
	if (isatty(STDIN_FILENO) == 1)
	{
		chdir(core.direct);
		pwd_update(&core);
		while (status == -1)
		{
			core.line = readline("PeePeeShell$ > ");
			add_history(core.line);
    	  	token = tokenize(&core);
			if(token)
			{	
				core.cmd = parse(&core, token);
				executor(core.cmd, &core);
			}
			free(core.line);
			if (status >= 0)
				exit(status);
		}
	}
	return (0);
}
