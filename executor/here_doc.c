/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:41:30 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/30 12:38:27 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
void	here_doc(t_cmdtable *cmd, int *fd)
{
	char *line;
	int tmp_fd[2];
	
	if (pipe(tmp_fd) == -1)
	{
		error_handler_fd(tmp_fd[1]);
		return ;
	}

	while (1)
	{
		line = readline("> ");
		if (!line)
			break;
		if (ft_strncmp(line, cmd->redir, ft_strlen(cmd->redir)) == 0 && 
			ft_strlen(line) == ft_strlen(cmd->redir))
		{
			free(line);
			break;
		}
		write(tmp_fd[1], line, strlen(line));
		write(tmp_fd[1], "\n", 1);
		free(line);
	}

	close(tmp_fd[1]);

	if (dup2(tmp_fd[0], STDIN_FILENO) == -1)
	{
		close(tmp_fd[0]);
		error_handler_fd(fd[1]);
		return;
	}
	close(tmp_fd[0]); 
	
	if (cmd->args[0] == NULL)
		exit(1);
}
