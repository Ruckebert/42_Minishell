/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:41:30 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/31 14:52:06 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *expander_env(t_data *core, char *line)
{
	//Change this code up to just copy itll the variable and strjoin them together
	//To Do: change this up
	char *expanded_line = NULL;
	char *var_start = ft_strchr(line, '$');
	if (!var_start)
		return (NULL);
		
	//int before_var_len = var_start - line;
	char *var_name = var_start + 1;
	char *env_value = getenv(var_name);
	if (!env_value)
		env_value = "";
	expanded_line = ft_strjoin(var_start,env_value);
	if (!expanded_line)
	{
		core->exit_status = 1;
		exit_com(core);
	}
	return (expanded_line);
}

void	here_doc(t_cmdtable *cmd, t_data *core, int *fd)
{
	char *line;
	char *expand_line;
	int tmp_fd[2];
	
	if (pipe(tmp_fd) == -1)
		error_handler_fd(tmp_fd[1]);

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
		expand_line = expander_env(core, line);
		if (expand_line)
		{
			write(tmp_fd[1], expand_line, strlen(expand_line));
			free(expand_line);
		}
		else
			write(tmp_fd[1], line, strlen(line));
		write(tmp_fd[1], "\n", 1);
		free(line);
	}
	close(tmp_fd[1]);

	if (dup2(tmp_fd[0], STDIN_FILENO) == -1)
	{
		close(tmp_fd[0]);
		error_handler_fd(fd[1]);
	}
	close(tmp_fd[0]);

	if (cmd->args[0] == NULL)
		exit(1);
}
