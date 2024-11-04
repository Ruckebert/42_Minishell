/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:58:57 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/04 14:44:02 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char *argv[], char **env)
{
	t_data core;
	t_token *token;
	
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
	if ( isatty(STDIN_FILENO) == 1 /*true*/) //The isatty is the reason why the tester doesnt work
	{
		//chdir(core.direct);
		//pwd_update(&core);
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
				exit_com(&core);
		}
	}
	return (0);
}
