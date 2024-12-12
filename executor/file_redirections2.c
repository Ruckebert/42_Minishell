/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_redirections2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:45:11 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/12 13:34:54 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_cmdtable	*input_types(t_cmdtable *cmd, t_data *core, t_var *vars, int fd)
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

t_cmdtable	*input_redirections(t_cmdtable *cmd, t_data *core,
	t_var *vars, int fd)
{
	t_cmdtable	*input;

	input = NULL;
	while (cmd)
	{
		if (cmd->redir_type != 2 && cmd->redir_type != 20)
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
	vars->file_error = 0;
	return (input);
}

t_cmdtable	*output_redirections(t_cmdtable *cmd, t_data *core,
	t_var *vars, int *fd)
{
	t_cmdtable	*output;

	output = NULL;
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

t_cmdtable	*both_redirections(t_cmdtable *cmd, t_data *core,
	t_var *vars, int fd)
{
	t_cmdtable	*input;

	input = NULL;
	input = input_redirections(cmd, core, vars, fd);
	if (access(input->redir, R_OK) != 0)
		file_input(input, core, vars, &fd);
	cmd->next = output_redirections(cmd, core, vars, &fd);
	cmd->redir_type = 40;
	cmd = input;
	vars->file_error = 0;
	return (cmd);
}

t_cmdtable	*multi_redirections(t_cmdtable *cmd, t_data *core, t_var *vars)
{
	t_cmdtable	*tmp;
	int			fd[2];

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
	else if (vars->input == 0 && vars->output != 0)
		return (output_redirections(cmd, core, vars, fd));
	return (NULL);
}
