/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:26:46 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/16 15:57:06 by aruckenb         ###   ########.fr       */
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

int	ft_strcmp(char *s1, char *s2)
{
	while ((*s1 != '\0') && (*s2 != '\0'))
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	return (0);
}

void	bubble_sort(t_data *core)
{
	int swapped;
	char *temp_str;
	int	i;
	
	i = 0;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (core->export_env[i + 1])
		{
			if (ft_strcmp(core->export_env[i], core->export_env[i + 1]) > 0)
			{
				temp_str = core->export_env[i];
				core->export_env[i] = core->export_env[i + 1];
				core->export_env[i + 1] = temp_str;
				swapped = 1;
			}
			i++;
		}
	}
}


int		environment_export(t_data *core)
{
	int count;

	count = 0;
	if (core->export_env[0] == NULL)
	{
		write(1, "2", 1);
		while (core->env[count])
		{
			core->export_env[count] = ft_strdup(core->env[count]);
			count++;
		}
		core->export_env[count] = NULL;
	}
	else
	{
		while (core->env[count])
			count++;
	}
	return (count);
}

void	print_exo_env(t_data *core)
{
	int i;
	
	i = 0;
	while (core->export_env[i])
	{
		ft_printf("declare -x %s\n", core->export_env[i]);
		i++;
	}
}

char	**new_exo_env(char **env, char **argv, int argc, int count)
{
	int j;
	int i;
	char **temp;
	temp = malloc((argc + count) * sizeof(char *));
	if (!temp)
		exit(1);

	i = 0;
	while (env[i])
	{
		temp[i] = ft_strdup(env[i]);
		i++;
	}
	j = 0;
	while (j < count)
	{
		temp[i] = ft_strdup(argv[j + 1]);
		i++;
		j++;
	}
	temp[i] = NULL;
	return (temp);
}
void	export(t_data *core)
{
	//Add a function to check whether or not the variable already exists or not
	int i = 0;
	int count = 0;
	char **temp;
	char **argv = ft_split(core->line, ' ');
	
	count = environment_export(core);
	bubble_sort(core);

	//Made to Check arguements 
	i = count + 1;
	count = 0;
	while (argv[count])
		count++;

	if (count == 1)
		print_exo_env(core);
	else if (count > 1)
	{
		int j = 0;
		
		while (j < count - 1)
		{
			i = 0;
			while (core->export_env[i])
			{
				if (ft_strcmp(core->export_env[i], argv[j + 1]) == 0)
				{	//Issue Does not Duplicate it anymore
					core->export_env[i] = ft_strdup(argv[j + 1]);
				}
				i++;
			}
			j++;
		}
		
		temp = new_exo_env(core->export_env, argv, i, count);
		if (!temp)
			exit(write(2, "Error: Enviornment is Not Sexy Enough\n", 39));
		core->env = new_exo_env(core->env, argv, i, count);
		if (!core->env)
			exit(write(2, "Error: Enviornment is Not Sexy Enough\n", 39));
		i = 0;
		while (core->export_env[i]) //I think i free nothing 
		{
			if (!core->export_env[i])
				free(core->export_env[i]);
			i++;
		}
		free(core->export_env);
		core->export_env = temp;
	}
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
	write(1, "1", 1);
}

void	unset(t_data *core)
{
	//Unset removes a variable in the environment and export environment
	//However if it is invaild nothing happens
	char	**argv = ft_split(core->line, ' ');
	//int		i;
	
	ft_printf("%s\n", argv[1]);

	/*This is wrong but it has the right idea
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
