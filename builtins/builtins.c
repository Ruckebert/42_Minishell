/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:26:46 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/19 13:19:37 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// All Builtins Manipulate the Environment that is why we have to build them seperately. :0 

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
	if (cmd->args[1] == NULL)
	{
		cd_empty(old_pwd, core);
		return ;
	}
	else if (cmd->args[1][0] == '-')
	{
		cd_oldpwd(old_pwd, core);
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
		{
			core->exit_status = 1; //An error message has to printed
			write(2, "Error: Enviornment is Not Sexy Enough\n", 39);
			return ;
		}
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
	int j;
	
	i = 1;
	j = 1;
	no = 0;
	
	if (cmd->args[i] == NULL)
		no = 0;
	else if (ft_strncmp(cmd->args[i], "-n", 2) == 0)
	{
		while (cmd->args[i][j])
		{
			if (cmd->args[i][j] == 'n')
				no = 1;
			else
				no = 0;
			j++;
		}
		if (no == 1)
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

//BTW Do not look at this?
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
				{
					core->exit_status = ft_atoi(core->cmd->args[1]) % 256;
				}
				else
				{
					j = -1;
					break ;
				}
			}
			else if (core->cmd->args[i][j] == '-' && j == 0)
			{
				if (core->cmd->args[i][j + 1] >= '0' && core->cmd->args[i][j + 1] <= '9')
					core->exit_status = ft_atoi(core->cmd->args[1]) % 256;
				else
				{
					j = -1;
					break ;
				}
			}
			else if (core->cmd->args[i][j] >= '0' && core->cmd->args[i][j] <= '9')
				core->exit_status = ft_atoi(core->cmd->args[1]) % 256;
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
	else if (j != -1 && core->cmd->args[2] == NULL)
	{
		//IDk why but it doesnt work for negative numbers
		//printf("%d\n", ft_atoi(core->cmd->args[1]));
		//printf("%d\n", 256 % ft_atoi(core->cmd->args[1]));
		if (ft_atoi(core->cmd->args[1]) < 0)
			core->exit_status = 256 - (ft_atoi(core->cmd->args[1]) * -1);
		else
			core->exit_status = ft_atoi(core->cmd->args[1]) % 256;
	}
	else if (j != -1 && core->cmd->args[2] != NULL)
	{
		write(2, "exit: too many arguments\n", 26);
		return ;
	}
	else if (j == -1)
	{
		write(2, "exit: numeric arugment required\n", 33);
		core->exit_status = 2; //Change to the correct exit status
	}
	
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
	//printf("Exit Status: %d\n", core->exit_status);
	exit(core->exit_status);
}
