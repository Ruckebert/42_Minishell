/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:05:49 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/16 11:38:41 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Builtin Commands

int		builtin_cmds(char *line, t_data *core)
{
	if (ft_strlen(line) == 0)
		return (-1);
	else if (ft_strncmp(line, "pwd", ft_strlen(line)) == 0)
		pwd(core);
	else if (ft_strncmp(line, "cd", 2) == 0)
		cd_com(core);
	else if (ft_strncmp(line, "exit", ft_strlen(line)) == 0)
		exit (1);
	else if (ft_strncmp(line, "export", 6) == 0)
		export(core);
	return (-1);
}

//Simply a Test Function to test certain things for the executor
//It needs alot of work though :(
int test(t_data *core)
{
	t_command cmd;

	char **split_cmd = ft_split(core->line, ' ');
	int i = 0;
	
	while (split_cmd[i] != NULL)
	{
		//ft_printf("%s\n", split_cmd[i]);
		i++;
	}
	
	cmd.args = split_cmd;
	cmd.name = split_cmd[0];
	cmd.here_doc = 0;
	cmd.here_doc_delimiter = NULL;
	cmd.arg_count = i;
	cmd.input_file = NULL;
	cmd.output_file = NULL;
	
	//ft_printf("%d\n", i);
	executor(&cmd, core);
	
	i = 0;
	while (split_cmd[i] != NULL)
	{
		free(split_cmd[i]);
		i++;
	}
	free(split_cmd);
	
	return (0);
}

int main(int argc, char *argv[], char **env)
{
	t_data core;
	int status = -1;

	if (argc == -1)
		exit(2);
	core.env = copy_env(env, &core);
	if (core.env == NULL)
		return (2);
	(void)argc;
	(void)argv;

	if (isatty(STDIN_FILENO) == 1)
	{
		chdir(core.direct);
		pwd_update(&core);
		while (status == -1)
		{
			ft_printf("PeePeeShell$ ");
			ft_printf("%s ", core.user);
			core.line = readline("> ");
			add_history(core.line);
			status = builtin_cmds(core.line, &core);
			test(&core);
			free(core.line);
			if (status >= 0)
				exit(status);
		}
	}
	return (0);
}
