/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:40:28 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/27 16:07:43 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	closing_cmds_parent(int cmds, int fd[cmds - 1][2])
{
	int j = 0;
	while (j < cmds - 1)
	{
		close(fd[j][0]);
		close(fd[j][1]);
		j++;
	}
}

int	cmd_count(t_cmdtable *cmd)
{
	int i;
	int total;

	i = 0;
	total = 0;
	while (cmd)
	{
		i = 0;
		if (cmd->args)
		{
			while (cmd->args[i])
			{
				i++;
				total++;
				break ;
			}
		}
		cmd = cmd->next;
	}
	return (total);
}

int	here_doc_counter(t_cmdtable *cmd)
{
	int total;

	total = 0;
	while (cmd)
	{
		if (cmd->redir_type == 10 || cmd->redir_type == 30)
			total++;
		cmd = cmd->next;
	}
	return (total);
}

void multi_pipe(t_var *vars, t_cmdtable *cmd, t_data *core, char **envp)
{
	char **files = NULL;
    int status = 0;
    int fd[2];
    int prev_fd = -1;
    int childids[cmd_count(cmd) + 1];
    int i = 0;
	int j;
    t_cmdtable *current_cmd = cmd;
	t_cmdtable *temp = current_cmd;

	if (here_doc_counter(cmd) != 0)
	{
		files = ft_calloc(here_doc_counter(cmd), sizeof(char *));
		while (current_cmd)
		{
			if (current_cmd->redir_type == 10 || current_cmd->redir_type == 30)
			{
				current_cmd->redir = ft_strdup(here_doc_tempfile(current_cmd, core, STDIN_FILENO));
				current_cmd->redir_type  = 1;
				files[i] = ft_strdup(current_cmd->redir);
				i++;
			}
			current_cmd = current_cmd->next;
		}
		current_cmd = temp;
	}
	i = 0;
    while (current_cmd)
    {
        if (current_cmd->next && pipe(fd) == -1)
            error_handler();
		if (current_cmd->redir_type != 0)
		{
			current_cmd = multi_redirections(current_cmd, core, vars);
			vars->file_error = 0;
		}
        vars->childid = fork();
        if (vars->childid == -1)
            error_handler();
        if (vars->childid == 0)
        {
			if (prev_fd != -1)
            {
                if (dup2(prev_fd, STDIN_FILENO) == -1)
                    error_handler_fd(prev_fd, current_cmd);
                close(prev_fd);
				if (current_cmd->redir_type == 40)
                	redirctions(current_cmd, core, vars, NULL);
            }
            if (current_cmd->next)
            {
                close(fd[0]);
                if (dup2(fd[1], STDOUT_FILENO) == -1)
                    error_handler_fd(fd[1], current_cmd);
                close(fd[1]);
            }
            if (current_cmd->redir_type != 0)
                redirctions(current_cmd, core, vars, NULL);
			if (current_cmd->isbuiltin == 1)
                echo_cmd(current_cmd, core);
            else if (current_cmd->isbuiltin != 0)
                builtin_cmds(current_cmd, core);
            else if (current_cmd->args && ft_strchr(current_cmd->args[0], '/'))
                absolute_path_finder(core, core->env, current_cmd->args);
            else
                path_finder(vars, core, envp, current_cmd->args, 0);
			exit(core->exit_status);
        }
        if (prev_fd != -1)
            close(prev_fd);
		if (current_cmd->next)
        {
            close(fd[1]);
            prev_fd = fd[0];
        }
        childids[i++] = vars->childid;
        current_cmd = current_cmd->next;
    }
    if (prev_fd != -1)
		close(prev_fd);

	j = -1;
	while (++j <= i)
	{
		waitpid(childids[++j], &status, 0);
	}
	if (WIFEXITED(status))
		core->exit_status = WEXITSTATUS(status);

	if (files)
	{	
		j = 0;
		while (files[j])
		{
			unlink(files[j]);
			j++;
		}
	}
	exit(core->exit_status);
}
