/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:26:46 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/11 12:46:16 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// All Builtins Manipulate the Environment that is why we have to build them seperately. :0 

void	cd_oldpwd(char *old_pwd, t_data *core)
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
	if (chdir(core->direct) == -1)
		core->exit_status = 1;
	envi_update(old_pwd, core);
	free(old_pwd);
}

void	cd_empty(char *old_pwd, t_data *core)
{
	int i;
	i = 0;
		
	free(core->direct);
	while (core->env[i])
	{
		if	(ft_strncmp(core->env[i], "HOME=", 5) == 0)
		{
			core->direct = ft_strdup(core->env[i] + 5);
			if (!core->user)
				return ;
			break ;
		}
		i++;
	}
	if (chdir(core->direct) == -1)
		core->exit_status = 1;
	envi_update(old_pwd, core);
	free(old_pwd);
}

void	normal_cd(char *old_pwd, t_cmdtable *cmd, t_data *core)
{
	free(core->direct);
	core->direct = ft_strdup(cmd->args[1]);
	if (access(core->direct, sizeof(char)) == 0)
	{
		if (chdir(core->direct) == -1)
			core->exit_status = 1;
		envi_update(old_pwd, core);
	}
	else
	{
		ft_printf("cd: %s: no such file or directory\n", core->direct);
		core->exit_status = 1;
	}
	free(old_pwd);
}

void	cd_com(t_cmdtable *cmd, t_data *core)
{
	char *old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (cmd->args[2] != NULL)
	{
		write(2, "cd: too many arguments\n", 24);
		core->exit_status = 1;
		free(old_pwd);
		return ;
	}
	if (cmd->args[1][0] == '-')
	{
		cd_oldpwd(old_pwd, core);
		return ;
}
	if (cmd->args[1] == NULL)
	{
		cd_empty(old_pwd, core);
		return ;
	}
	normal_cd(old_pwd, cmd, core);
	return ;
}

void	pwd(t_data *core)
{
	free(core->direct);
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

void	simple_free(char **str)
{
	int i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
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
		simple_free(core->export_env);
		core->export_env = temp;
	}
}

void	unset(t_cmdtable *cmd, t_data *core)
{
	char	**temp;
	int		i;
	
	i = 0;
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
}

void	echo_cmd(t_cmdtable *cmd, t_data *core)
{
	int	i;
	int no;
	
	i = 1;
	no = 0;
	
	if (cmd->args[i] == NULL)
		no = 0;
	else if (ft_strcmp(cmd->args[i], "-n") == 0)
	{
		no = 1;
		i++;
	}
	while (cmd->args[i])
	{
		if (cmd->args[i + 1] == NULL)
			ft_printf("%s", cmd->args[i]);
		else
			ft_printf("%s ", cmd->args[i]);
		i++;
	}
	if (no == 0)
		ft_printf("\n");
	exit(core->exit_status);
}

//To Do: Exit command should free everything and then exit;
//Need to free the cmd and everything in it
void	exit_com(t_data *core)
{
	int i = 1;
	int j = 0;
	
	//I need to check whether or not the value is numerical or not
	//I should only check that their is a single argument, if its more then one display too many arguments and do NOT exit
	//So if the number is positive and not over 256 then just print that number out
	//If the number is positive and is over subtract it with 256
	//For negative numbers subtract it with 256
	//If the number is 256 the exit status is 0  
	
	while (core->cmd->args[i])
	{
		j = 0;
		while (core->cmd->args[i][j])
		{
			if (core->cmd->args[i][j] == '+' && j == 0)
			{
				if (core->cmd->args[i][j + 1] >= '0' && core->cmd->args[i][j + 1] <= '9')
				{}
				else
				{
					j = -1;
					break ;
				}
			}
			else if (core->cmd->args[i][j] == '-' && j == 0)
			{
				if (core->cmd->args[i][j + 1] >= '0' && core->cmd->args[i][j + 1] <= '9')
				{}
				else
				{
					j = -1;
					break ;
				}
			}
			else if (core->cmd->args[i][j] >= '0' && core->cmd->args[i][j] <= '9')
			{}
			else
			{
				j = -1;
				break ;
			}
			j++;
		}
		if (j == -1)
			break ;
		i++;	
	}

	//checks whether or not all the requirements are met
	if (j != -1 && core->cmd->args[1] == NULL)
	{
		
	}
	else if (j != -1 && core->cmd->args[2] != NULL)
	{
		write(2, "exit: too many arguments\n", 26);
		return ;
	}
	else if (j == -1)
		write(2, "exit: numeric arugment required\n", 33);
	
	//Frees everything and exits
	i = 0;
	while (core->env[i])
		i++;
	simple_free(core->env);
	if (core->export_env != NULL)
		simple_free(core->export_env);
	free(core->user);
	free(core->direct);
	free(core->line);
	if (core->cmd != NULL)
		free_cmdtable(&core->cmd);
	rl_clear_history();
	exit(core->exit_status);
}
