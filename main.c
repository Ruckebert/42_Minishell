/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:05:49 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/10 15:01:04 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		execute_args(char *line, t_data *core)
{
	if (ft_strlen(line) == 0)
		return (-1);
	else if (ft_strncmp(line, "pwd", ft_strlen(line)) == 0)
	{
		core->direct = getcwd(NULL, 0);
		ft_printf("%s\n", core->direct);
	}
	else if (ft_strncmp(line, "cd", 2) == 0)
		cd_com(core);
	else if (ft_strncmp(line, "exit", ft_strlen(line)) == 0)
		return (0);
	return (-1);
}

char **copy_env(char **env, t_data *core)
{
	char **new_env;
	int count;
	int i;

	if (env == NULL || *env == NULL)
		exit(1);
	
	count = 0;
	while (env[count] != NULL)
		count++;
	
	new_env = malloc((count + 1) * sizeof(char *));
	if (!new_env)
		exit(1);
	
	i = 0;
	while (i < count)
	{
		new_env[i] = ft_strdup(env[i]);
		if (ft_strncmp(new_env[i], "USER=", 5) == 0)
			core->user = ft_strdup(env[i] + 5);
		if	(ft_strncmp(new_env[i], "HOME=", 5) == 0)
			core->direct = ft_strdup(env[i] + 5);
		if (!new_env[i])
			exit(1); //If this fails free everything before
		i++;
	}
	new_env[count] = NULL;
	return (new_env);
}

int main(int argc, char *argv[], char **env)
{
	t_data core;
	int status = -1;

	if (argc == -1)
		exit(1);
	core.env = copy_env(env, &core);
	(void)argc;
	(void)argv;

	if (isatty(STDIN_FILENO) == 1)
	{
		chdir(core.direct);
		while (status == -1)
		{

			ft_printf("PeePeeShell$ ");
			ft_printf("%s ", core.user);
			core.line = readline("Input > ");
			status = execute_args(core.line, &core); //Executor
			free(core.line);
			if (status >= 0)
				exit(status);
		}
	}
	else
	{
		//No Minishell
	}
	return (0);
}