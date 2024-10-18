/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:26:46 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/18 11:24:52 by aruckenb         ###   ########.fr       */
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
	{
		free(core->direct);
		return ;
	}
	ft_printf("%s\n", core->direct);
}

void	env(t_data *core)
{
	int i;
	int j;
	
	i = 0;
	while (core->env[i] != NULL)
	{
		j = 0;
		while (core->env[i][j])
		{
			if (core->env[i][j] == '=')
			{
				ft_printf("%s\n",core->env[i]);
				break ;
			}
			j++;
		}
		i++;
	}
	return ;
}


//All Functions needed for Export 

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

int	len_env_var(char **argv, int j)
{
	int i;
	
	i = 0;
	while (argv[j][i])
	{
		if (argv[j][i] == '=')
			break ;
		i++;
	}
	return (i);
}

int	len_env_var_unset(char **argv, int j);

int	check_dup_exo(char **env, char **argv, char **temp, int j)
{
	int k;
	int found;
	int var_len;
	int	env_len;
	
	k = 0;
	found = 0;
	var_len = 0;
	env_len = 0;
	while (env[k])
	{
		var_len = len_env_var(argv, j);
		env_len = len_env_var_unset(temp, k);
		if (var_len == env_len)
		{
			if (ft_strncmp(temp[k], argv[j], var_len) == 0)
			{
				temp[k] = ft_strdup(argv[j]);
				found = 1;
			}
		}
		k++;
	}
	return (found);
}
char	**new_exo_env(char **env, char **argv, int argc, int count)
{
	int j;
	int i;
	int found;
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
	j = 1;
	while (argv[j])
	{
		found = 0;
		found = check_dup_exo(env, argv, temp, j);
		if (!found)
		{
			temp[i] = ft_strdup(argv[j]);
			i++;
		}
		j++;
	}
	temp[i] = NULL;
	return (temp);
}
void	export(t_data *core)
{
	int i = 0;
	int count = 0;
	char **temp;
	char **argv = ft_split(core->line, ' ');
	
	count = environment_export(core);
	bubble_sort(core);
	i = count + 1;
	count = 0;
	while (argv[count])
		count++;
	if (count == 1)
		print_exo_env(core);
	else if (count > 1)
	{	
		temp = new_exo_env(core->export_env, argv, i, count);
		if (!temp)
			exit(write(2, "Error: Enviornment is Not Sexy Enough\n", 39));
		core->env = new_exo_env(core->env, argv, i, count);
		if (!core->env)
			exit(write(2, "Error: Enviornment is Not Sexy Enough\n", 39));
		i = 0;
		while (core->export_env[i]) //I think i free nothing 
		{
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
}

//All Unset Functions
//To Do: Delete this function and replace it with the other one
int	len_env_var_unset(char **argv, int j)
{
	int i;
	
	i = 0;
	while (argv[j][i])
	{
		if (argv[j][i] == '=')
			break ;
		i++;
	}
	return (i);
}

int		finder(int found, int i, char **argv, char **env)
{
	int argc;
	int var_len = 0;
	int	env_len = 0;

	argc = 1;
	while (argv[argc])
	{
		var_len = len_env_var_unset(argv, argc);
		env_len = len_env_var_unset(env, i);
		if (var_len == env_len)
		{
			if (ft_strncmp(env[i], argv[argc], var_len) == 0)
			{
				found = 1;
				break ;
			}
		}
		argc++;
	}
	return (found);
}

char	**unset_exo(t_data *core, char **env, int i, char **argv)
{
	int count = environment_export(core);
	char **temp;
	int found = 0;
	
	temp = malloc(((count - i) + 1) * sizeof(char *));
	if (!temp)
		exit(write(1, "Malloc Error", 13));
	i = 0;
	int k = 0;
	while (env[i])
	{
		found = 0;
		found = finder(found, i, argv, env);
		if (!found)
		{
			temp[k] = ft_strdup(env[i]);
			k++;
		}	
		i++;
	}
	temp[k] = NULL;
	return (temp);
}

char	**unset_env(t_data *core, char **env, int i, char **argv)
{
	int new_env;
	int found = 0;
	char **temp;
	temp = malloc(((environment_export(core) - i) +  1) * sizeof(char *));
	if (!temp)
		exit(1);

	new_env = 0;
	i = 0;
	while (env[i])
	{
		found = 0;
		found = finder(found, i, argv, env);
		if (!found)
		{
			temp[new_env] = ft_strdup(env[i]);
			new_env++;
		}
		i++;
	}
	temp[new_env] = NULL;
	return (temp);
}

void	unset(t_data *core)
{
	char	**argv;
	char	**temp;
	int		i;
	
	i = 0;
	argv = ft_split(core->line, ' ');
	while (argv[i])
		i++;
	if (i == 1)
		return ;
	temp = unset_exo(core, core->export_env, i, argv);
	core->env = unset_env(core, core->env, i, argv);
	i = 0;
	while (core->export_env[i])
	{
		if (!core->export_env[i])
			free(core->export_env[i]);
		i++;
	}
	free(core->export_env);
	core->export_env = temp;
}


//Builtins that still need work
void	echo_cmd(t_data *core)
{
	char	**argv = ft_split(core->line, ' ');
	ft_printf("%s\n", argv[1]);
}

//To Do: Exit command should free everything and then exit;
void	exit_com()
{
	exit(1);
}
