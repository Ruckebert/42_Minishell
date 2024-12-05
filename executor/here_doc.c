/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:41:30 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/05 15:29:09 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t g_interrupt_received;

void	here_doc_null_msg(t_cmdtable *cmd)
{
	ft_putstr_fd("warning: here-document delimited by end-of-file ", 2);
	ft_putstr_fd("(wanted `", 2);
	ft_putstr_fd(cmd->redir, 2);
	ft_putstr_fd("')\n", 2);
}


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

char *ft_nbr_pointhex(intptr_t num)
{
	int					store;
	unsigned long		nb;
	int					count;
	char				*character;

	character = ft_calloc(21, sizeof(char *));
	nb = num;
	count = 0;
	while (nb > 0)
	{
		store = nb % 16;
		if (store < 10)
			character[count++] = 48 + store;
		else
			character[count++] = 87 + store;
		nb = nb / 16;
	}
	character[count] = '\0';
	return (character);
}

char	*here_doc_tempfile(t_cmdtable *cmd, t_data *core, int fd)
{
	char *line;
	char *expand_line;
	char *filename;
	int tmp_fd;

	//setup_signal_handler(SIGINT, sig_handleINT_parent2);
	//setup_signal_handler(SIGINT, SIG_DFL);
	setup_signal_handler(SIGINT, sig_handleINT_heredoc);
	filename = ft_nbr_pointhex((intptr_t)cmd->redir);
	tmp_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (tmp_fd == -1)
		error_handler_fd(fd, cmd);
	while (1)
	{
		line = readline("> ");
		if (g_interrupt_received != 0)
			return (NULL);
		if (!line)
		{
			here_doc_null_msg(cmd);
			break ;
		}

		if (ft_strncmp(line, cmd->redir, ft_strlen(cmd->redir)) == 0 && 
			ft_strlen(line) == ft_strlen(cmd->redir))
		{
			free(line);
			break;
		}
		expand_line = expander_env(core, line);
		if (expand_line && cmd->redir_type == 10 )
		{
			write(tmp_fd, expand_line, strlen(expand_line));
			free(expand_line);
		}
		else
			write(tmp_fd, line, strlen(line));
		write(tmp_fd, "\n", 1);
		free(line);
	}
	close(tmp_fd);
	return (filename);
}
