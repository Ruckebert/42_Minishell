/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:58:57 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/24 12:54:50 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Simply a Test Function to test certain things for the executor
//It needs alot of work though :(
int test(t_data *core)
{
	t_cmdtable cmd1;
	t_cmdtable cmd2;

  // First command "cat infile"
    char one[] = "cat";
    char two[] = "infile";
    
    cmd1.args = malloc (sizeof(char*) * 3);
    cmd1.args[0] = one;
    cmd1.args[1] = two;
    cmd1.args[2] = NULL;
    cmd1.has_pipe_after = 1;  // There's a pipe after this command
    cmd1.redir_type = 0;
    cmd1.redir = NULL;
    cmd1.next = &cmd2;        // Point to the next command in the pipeline
    cmd1.prev = NULL;

    // Second command "grep searchterm"
    char three[] = "grep";
    char four[] = "Poop";
    
    cmd2.args = malloc (sizeof(char*) * 3);
    cmd2.args[0] = three;
    cmd2.args[1] = four;
    cmd2.args[2] = NULL;
    cmd2.has_pipe_after = 0;  // No pipe after this command
    cmd2.redir_type = 0;
    cmd2.redir = NULL;
    cmd2.next = NULL;         // This is the last command
    cmd2.prev = &cmd1;        // Point back to the previous command
	
	executor(&cmd1, core);
	return (0);
}

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
		exit_com(core);
	else if (ft_strncmp(line, "export", 6) == 0)
		export(core);
	else if (ft_strncmp(line, "unset", 4) == 0)
		unset(core);
	else if (ft_strncmp(line, "env", 3) == 0)
		env(core);
	else if (ft_strncmp(line, "echo", 4) == 0)
		echo_cmd(core);
	else if (ft_strlen(line) > 1)
		test(core);
	return (-1);
}

int main(int argc, char *argv[], char **env)
{
	t_data core;
	//t_token *token;
	
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
      		//token = tokenize(&core);
			//parse(&core, env, token);
			status = builtin_cmds(core.line, &core);
			free(core.line);
			if (status >= 0)
				exit(status);
		}
	}
	return (0);
}
