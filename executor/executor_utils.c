/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 13:09:11 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/06 14:34:38 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	error_handler(void)
{
	perror("A Error Has occured");
	exit(1);
}

void	error_handler_fd(int fd, t_cmdtable *cmd)
{
	close(fd);
	ft_putstr_fd(cmd->redir, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	return ;
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

void	here_doc_file_del(char **files)
{
	int	i;

	i = 0;
	if (files)
	{
		while (files[i])
		{
			unlink(files[i]);
			i++;
		}
	}
}
