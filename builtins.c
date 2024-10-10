/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:26:46 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/10 14:55:45 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd_com(t_data *core)
{
	core->direct = ft_strdup(core->line + 3);
	if (access(core->direct, sizeof(char)) == 0)
		chdir(core->direct);
	else
		ft_printf("cd: no such file or directory: %s\n", core->direct);
	return ;
}

void	pwd()
{

}