/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:09:11 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/25 13:01:42 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_handler(void)
{
	perror("A Error Has occured");
	exit(1);
}

void	error_handler_fd(int fd)
{
	close(fd);
	perror("Error in reading file or duplication");
	exit(1);
}

void	error_handler_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
	perror("A Error Has Occured in the split");
	exit(1);
}

void	free_split(char **split)
{
	int	i;

	i = 0;
	if (!split)
		return ;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

void	path_finder_error(char **cmd)
{
	int i = 0;
	
	if (cmd)
	{
		while(cmd[i])	
		{
			free(cmd[i]);
			i++;
		}
		free(cmd[i]);
	}
	//free_split(cmd);
}
