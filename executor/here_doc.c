/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:41:30 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/14 10:58:08 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *expander_env(t_data *core, char *line) 
{
    char *expanded_line = NULL;
    char *var_start = NULL;
    char *env_value = NULL;
	char *var_name = NULL;
	char *before_var = NULL;
    int i = 0;
	int j = 0;
	core->exit_status = 1; //in case of an error change the exit status

    while (line[i]) 
	{
        if (line[i] == '$') 
		{
            before_var = ft_substr(line, 0, i);
            expanded_line = ft_strjoin(expanded_line, before_var);
            free(before_var);
            var_start = &line[i + 1];
            j = 0;
            while (var_start[j] && (ft_isalnum(var_start[j]) || var_start[j] == '_'))
                j++;
            var_name = ft_substr(var_start, 0, j);
			if (!var_name)
			{
				free(var_start);
				free(expanded_line);
				exit(1);
			}
            env_value = getenv(var_name);
            free(var_name);
            if (!env_value) 
                env_value = "";
            expanded_line = ft_strjoin(expanded_line, env_value);
			if (!expanded_line)
			{
				free(var_start);
				free(expanded_line);
				free(env_value);
				exit(1);
			}
			i += j + 1;
            line = &line[i];
            i = 0;
        } 
		else 
            i++;
    }
    if (*line)
	{
        expanded_line = ft_strjoin(expanded_line, line);
		if (!expanded_line)
		{
			free(var_start);
			free(env_value);
			exit (1);
		}	
	}
    return (expanded_line);
}


void	here_doc(t_cmdtable *cmd, t_data *core, int fd)
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
	if (dup2(tmp_fd[0], fd) == -1)
	{
		close(tmp_fd[0]);
		error_handler_fd(fd);
	}
	close(tmp_fd[0]);
}
