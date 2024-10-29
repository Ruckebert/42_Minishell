/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:26:46 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/29 10:05:50 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// All Builtins Manipulate the Environment that is why we have to build them seperately. :0 
void	cd_com(t_cmdtable *cmd, t_data *core)
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
	core->direct = ft_strdup(cmd->args[1]);
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

void	export(t_cmdtable *cmd, t_data *core)
{
	int i = 0;
	int count = 0;
	char **temp;
	
	count = environment_export(core);
	bubble_sort(core);
	i = count + 1;
	count = 0;
	while (cmd->args[count])
		count++;
	if (count == 1)
		print_exo_env(core);
	else if (count > 1)
	{	
		temp = new_exo_env(core->export_env, cmd->args, i, count);
		if (!temp)
			exit(write(2, "Error: Enviornment is Not Sexy Enough\n", 39));
		core->env = new_exo_env(core->env, cmd->args, i, count);
		if (!core->env)
			exit(write(2, "Error: Enviornment is Not Sexy Enough\n", 39));
		i = 0;
		while (core->export_env[i])
		{
			free(core->export_env[i]);
			i++;
		}
		free(core->export_env);
		core->export_env = temp;
	}
}


void	unset(t_cmdtable *cmd, t_data *core)
{
	char	**argv;
	char	**temp;
	int		i;
	
	i = 0;
	argv = ft_split(core->line, ' ');
	if (strcmp(cmd->args[0], argv[0]) == 0)
		i = 0;
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

/*void	unset(t_cmdtable *cmd, t_data *core)
{
	//Ask Martin how he splits the input VARS for unset
	Yeh for some reason 
	char	**temp;
	int		i;
	
	i = 0;
	while (cmd->args[i])
	{
		ft_printf("%s\n", cmd->args[i]);
		i++;
	}
	if (i == 1)
		return ;
	temp = unset_exo(core, core->export_env, i, cmd->args);
	core->env = unset_env(core, core->env, i, cmd->args);
	i = 0;
	while (core->export_env[i])
	{
		if (!core->export_env[i])
			free(core->export_env[i]);
		i++;
	}
	free(core->export_env);
	core->export_env = temp;
}*/

//To Do: Requires Struct From parser for completion 
void	echo_cmd(t_cmdtable *cmd, t_data *core)
{
	int	i;
	int no;
	
	i = 1;
	no = 0;

	if (core->line == NULL) //A temporay statement to compile
		return ;
	if (ft_strcmp(cmd->args[i], "-n") == 0)
	{
		no = 1;
		i++;
	}
	while (cmd->args[i])
	{
		ft_printf("%s", cmd->args[i]);
		i++;
	}
	if (no == 0)
		ft_printf("\n");
}

//To Do: Exit command should free everything and then exit;
//Need to free the cmd and everything in it
void	exit_com(t_data *core)
{
	int i = 0;
	while (core->env[i])
		i++;
	int count = 0;
	while (count < i)
	{
		free(core->env[count]);
		count++;
	}
	free(core->env);
	count = 0;
	while (core->export_env[count])
	{
		free(core->export_env[count]);
		count++;
	}
	free(core->export_env);
	free(core->user);
	free(core->direct);
	free(core->line);
	rl_clear_history();
	exit(1);
}
