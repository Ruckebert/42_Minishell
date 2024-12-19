/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:08:59 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/19 09:58:59 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exit_return_checker(t_data *core, int i, int j)
{
	if (core->cmd->args[i][j + 1] >= '0'
		&& core->cmd->args[i][j + 1] <= '9' && j == 0)
		core->exit_status = ft_atoi(core->cmd->args[1]) % 256;
	else if (core->cmd->args[i][j] >= '0'
		&& core->cmd->args[i][j] <= '9')
		core->exit_status = ft_atoi(core->cmd->args[1]) % 256;
	else
		return (-1);
	return (0);
}

int	exit_condition_checker(t_data *core, int i, int j)
{
	if ((core->cmd->args[i][j] == '+' && j == 0)
		|| (core->cmd->args[i][j] == '-' && j == 0)
		|| (core->cmd->args[i][j] == '-'
		&& core->cmd->args[i][j - 1] == ' '))
	{
		if (exit_return_checker(core, i, j) == -1)
			return (1);
	}
	else if (exit_return_checker(core, i, j) == -1)
		return (1);
	return (0);
}

int	exit_loop(t_data *core, int i, int j)
{
	unsigned long long	sign;

	while (core->cmd->args[i] && i != 2)
	{
		j = 0;
		sign = ft_strtoull(core->cmd->args[1], &j, 0);
		if (j < 0)
			return (-1);
		while (core->cmd->args[i][j])
		{
			if (exit_condition_checker(core, i, j) == 1)
			{
				j = -1;
				break ;
			}
			j++;
		}
		if (j == -1)
			return (-1);
		i++;
	}
	if (core->cmd->args[1] != NULL && core->cmd->args[1][0] == '\0')
		return (-1);
	return (0);
}

int	exit_error_handler(int j, t_data *core, t_cmdtable *cmd)
{
	if (j != -1 && cmd->args[1] == NULL)
	{
	}
	else if (j != -1 && cmd->args[2] == NULL)
	{
		if (ft_atoi(cmd->args[1]) < 0)
			core->exit_status = 256 - (ft_atoi(cmd->args[1]) * -1);
		else
			core->exit_status = ft_atoi(cmd->args[1]) % 256;
	}
	else if (j != -1 && cmd->args[2] != NULL)
	{
		core->exit_status = 1;
		ft_putstr_fd("exit: too many arguments\n", 2);
		return (1);
	}
	else if (j == -1)
	{
		ft_putstr_fd("exit: ", 2);
		ft_putstr_fd(cmd->args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		core->exit_status = 2;
	}
	return (0);
}

void	exit_com(t_data *core, t_cmdtable *cmd)
{
	int		i;
	int		j;
	char	*temp;

	i = 1;
	j = 0;
	temp = NULL;
	if (cmd->args[1])
	{
		temp = ft_strtrim(cmd->args[1], " ");
		if (!temp)
			export_malloc_error(core, NULL);
		free(cmd->args[1]);
		cmd->args[1] = ft_strdup2(temp);
		free(temp);
	}
	j = exit_loop(core, i, j);
	if (exit_error_handler(j, core, cmd) == 1)
		return ;
	if (core->empty_cd == 3)
		write(2, "exit\n", 5);
	free_exit(core);
	exit(core->exit_status);
}
