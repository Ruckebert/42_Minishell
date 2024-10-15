/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:26:46 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/15 14:48:37 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// All Builtins Manipulate the Environment that is why we have to build them seperately. :0 

void	cd_com(t_data *core) // To Do: In the case something fails i have not free anything 
{
	char *old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (ft_strncmp(core->line, "cd -", 4) == 0)
	{
		int i = 0;
		while (core->env[i])
		{
			if (ft_strncmp(core->env[i], "OLDPWD=", 7) == 0)
			{
				free(core->direct);
				core->direct = ft_strdup(core->env[i] + 7);
				break ;
			}
			i++;
		}
		ft_printf("%s\n", core->direct);
		chdir(core->direct);
		envi_update(old_pwd, core);
		return ;
	}
	free(core->direct);
	core->direct = ft_strdup(core->line + 3);
	if (access(core->direct, sizeof(char)) == 0)
	{
		chdir(core->direct);
		envi_update(old_pwd, core);
	}
	else
		ft_printf("cd: %s: no such file or directory\n", core->direct);
	return ;
}

void	pwd(t_data *core)
{
	core->direct = getcwd(NULL, 0);
	if (!core->direct)
	{ //A Basic free if it fails 
		free(core->direct);
		return ;
	}
	ft_printf("%s\n", core->direct);
}

void	env(t_data *core)
{
	//The Env should print everything with an = and if it doesnt it will not
	int i;
	
	i = 0;
	while (core->env[i] != NULL)
	{
		ft_printf("%s\n",core->env[i]);
		i++;
	}
	return ;
}

void	export(t_data *core)
{
	//Export has its own environment where its in ascii order
	//When you Export something with out a variable so like this VAR1 then its not shown in the environemtn but shown in export
	//If it has a variable VAR1=1 then its shown in both the environment and export
	
	//int i = 0;
	char **argc = ft_split(core->line, ' ');
	ft_printf("%s\n", argc[0]);

	/*	Pseudo Code Frame Work
	while (env)
		count++;
	
	//Bubble Sort
	while (1)
	{
		While (env)
		{
			if (First in ascii, letter = 65)
			{
				copy it into the core->export_env
			}
			i++;
		}
	
	}
	if (argc == 1)
	{
		while (core->export_env)
		{
			ft_printf("declare -x %s\n", core->export_env[i])
			i++;
		}
	}
	if (argc > 1)
	{
		Export_env gets the new variables copied into it, in its correct ascii order
		core->env also gets it but at the end.
	}
	*/
}

void	unset(t_data *core)
{
	//Unset removes a variable in the environment and export environment
	//However if it is invaild nothing happens
	char	**argv = ft_split(core->line, ' ');
	//int		i;
	
	ft_printf("%s\n", argv[1]);

	/* This is wrong but it has the right idea
	while (core->env[i])
	{
		if (ft_strncmp(core->env[i], argv[1],ft_strlen(argv[1])) == 0)
		{
			//In this case you remove it in both the export environment and environment
			free(core->direct);
			core->direct = ft_strdup(core->env[i] + 7);
			break ;
		}
		i++;
	}*/
}

void	echo_cmd(t_data *core)
{
	char	**argv = ft_split(core->line, ' ');
	ft_printf("%s\n", argv[1]);
}

void	exit_com() //To Do: Exit command should free everything and then exit;
{
	exit(1);
}
