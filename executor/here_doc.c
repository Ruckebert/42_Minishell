/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_shit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:41:30 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/25 13:01:46 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
void	here_doc(t_cmdtable *cmd, int *fd)
{
	fd[0] = 1;
	cmd->args[1] = "FUCK";
	/*char *line;
	int tmp_fd[2];
	
	if (pipe(tmp_fd) == -1)
		error_handler_fd(tmp_fd[1]);

	while (1)
	{
		line = get_next_line(tmp_fd[1]);
		if (!line || ft_strncmp(line, cmd->here_doc_delimiter, ft_strlen(line)) == 0)
			break ;
		write(tmp_fd[1], line, ft_strlen(line));
		free(line);
	}
	close(tmp_fd[1]);
	if (dup2(tmp_fd[0], STDIN_FILENO) == -1)
	{
		close(tmp_fd[0]);
		error_handler_fd(fd[1]);
	}
	close(tmp_fd[0]);*/
}