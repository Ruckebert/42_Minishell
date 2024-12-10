/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:07:32 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/10 09:59:29 by aruckenb         ###   ########.fr       */
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
	/*Freeing Part*/
	simple_free(core->env);
	if (core->export_env != NULL)
		simple_free(core->export_env);
	free(core->user);
	free(core->direct);
	free(core->line);
	/*Free Part*/
	exit(core->exit_status);
}
