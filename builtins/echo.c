/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:07:32 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/15 09:57:06 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	second_no(int *no, int i, t_cmdtable *cmd)
{
	int	j;

	j = 0;
	i++;
	while (cmd->args[i])
	{
		if (ft_strncmp(cmd->args[i], "-n", 2) == 0)
		{
			j = 2;
			while (cmd->args[i][j] == 'n')
				j++;
			if (cmd->args[i][j] == '\0')
			{
				*no = 1;
				i++;
				continue ;
			}
		}
		break ;
	}
	return (i);
}

void	echo_exit(int i, int no, t_cmdtable *cmd, t_data *core)
{
	core->exit_status = 0;
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
	free_exit(core);
	exit(core->exit_status);
}

void	echo_cmd(t_cmdtable *cmd, t_data *core, int i)
{
	int	no;
	int	j;

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
			{
				no = 0;
				break ;
			}
			j++;
		}
		if (no == 1)
			i = second_no(&no, i, cmd);
	}
	echo_exit(i, no, cmd, core);
}
