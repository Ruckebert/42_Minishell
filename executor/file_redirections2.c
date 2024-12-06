/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_redirections2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:45:11 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/06 11:11:11 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmdtable *input_types(t_cmdtable *cmd, t_data *core, t_var *vars, int fd)
{
	if (cmd->redir_type == 1)
	{
		if (access(cmd->redir, R_OK) == 0)
			return (cmd);
		else
			return (NULL);
	}
	if (cmd->redir_type == 10 || cmd->redir_type == 30)
	{
		if (vars->filename != NULL)
			unlink(vars->filename);
		vars->filename = here_doc_tempfile(cmd, core, fd);
		return (cmd);
	}
	vars->file_error = 1;
	return (NULL);
}

t_cmdtable *input_redirections(t_cmdtable *cmd, t_data *core, t_var *vars, int fd)
{
	t_cmdtable *input;

	while (cmd)
	{
		input = input_types(cmd, core, vars, fd);
		if (!input && vars->file_error != 1)
			return (cmd);
		if (cmd->has_pipe_after == 1)
			break ;
		cmd = cmd->next;
	}
	if (input->redir_type == 10 || input->redir_type == 30)
	{
		input->redir = ft_strdup(vars->filename);
		input->redir_type = 1;
	}
	else
	{
		if (vars->filename != NULL)
			unlink(vars->filename);
	}
	vars->file_error = 0;
	return (input);
}

t_cmdtable *output_redirections(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd)
{
	t_cmdtable *output = NULL;
	
	while (cmd)
	{
		if (cmd->redir_type == 2 || cmd->redir_type == 20)
		{
			output = cmd;
			if (cmd->next == NULL || cmd->has_pipe_after == 1)
				break ;
			else
			{
				vars->file_error = 1;
				redirctions(cmd, core, vars, fd);
			}
		}
		if (cmd->has_pipe_after == 1)
			break ;
		cmd = cmd->next;
	}
	vars->file_error = 0;
	return (output);
}

t_cmdtable *both_redirections(t_cmdtable *cmd, t_data *core, t_var *vars, int fd)
{
	t_cmdtable *input;
	
	input = input_redirections(cmd, core, vars, fd);
	cmd->next = output_redirections(cmd, core, vars, &fd);
	cmd->redir_type = 40;
	cmd = input;
	vars->file_error = 0;
	return (cmd);
}

int	redirection_checker(t_cmdtable *cmd, t_var *vars)
{
	if (cmd->redir_type == 1 || cmd->redir_type == 10 || cmd->redir_type == 30)
		vars->input++;
	if (cmd->redir_type == 2 || cmd->redir_type == 20)
		vars->output++;
	if (cmd->has_pipe_after == 1)
		return (1);
	if (cmd->next == NULL)
		return (1);
	return (0);
}


t_cmdtable *multi_redirections(t_cmdtable *cmd, t_data *core, t_var *vars)
{
	t_cmdtable *tmp;
	int fd[2];

	tmp = cmd;
	vars->input = 0;
	vars->output = 0;
	vars->file_error = 0;
	while (cmd)
	{
		if (redirection_checker(cmd, vars) == 1)
			break ;
		cmd = cmd->next;
	}
	cmd = tmp;
	if (vars->input != 0 && vars->output != 0)
		return (both_redirections(cmd, core, vars, *fd));
	else if (vars->input != 0 && vars->output == 0)
		return (input_redirections(cmd, core, vars, *fd));
	else if (vars->input == 0 && vars->fdout != 0)
		return (output_redirections(cmd, core, vars, fd));
	return (NULL);
}
