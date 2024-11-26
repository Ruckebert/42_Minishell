/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:40:28 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/26 15:10:41 by aruckenb         ###   ########.fr       */
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
		while (cmd->args[i])
		{
			i++;
			total++;
			break ;
		}
		cmd = cmd->next;
	}
	//printf("Total:%d\n", total);
	return (total);
}
void	first_pipe(t_var *vars, t_data *core, t_cmdtable *cmd, int fd)
{
	if (cmd->redir_type != 0 && cmd->redir_type != 10 && cmd->redir_type != 30)
		redirctions(cmd, core, vars, &fd);
	if (dup2(fd, STDOUT_FILENO) == -1)
		error_handler_fd(fd, cmd);
}

void	last_pipe(t_var *vars, t_data *core, t_cmdtable *cmd, int fd)
{
	if (dup2(fd, STDIN_FILENO) == -1)
		error_handler_fd(fd, cmd);
	if (cmd->redir_type != 0 && cmd->redir_type != 10 && cmd->redir_type != 30)
		redirctions(cmd, core, vars, &fd);
}

void	multi_pipe(t_var *vars, t_cmdtable *cmd, t_data *core, char **envp)
{
	int		cmds = cmd_count(cmd);
	int		status = 0;
	int		i = 0;
	int		j = 0;
	int fd[cmds - 1][2];
	int childids[cmds];
	
	t_cmdtable *current_cmd = cmd;
	t_cmdtable *temp = current_cmd;
	
	//Here_doc file creation
	while (current_cmd)
	{
		if (current_cmd->redir_type == 10 || current_cmd->redir_type == 30)
		{
			current_cmd->redir = ft_strdup(here_doc_tempfile(current_cmd, core, STDIN_FILENO));
			current_cmd->redir_type  = 1;
		}
		current_cmd = current_cmd->next;
	}
	current_cmd = temp;
	
	//Piping process
	i = 0;
	j = 0;
	while (i < cmds - 1)
	{
		if (pipe(fd[i]) == -1)
		{
			write(1, "ERROR\n", 7);
			while (j < i)
			{
				close(fd[j][0]);
				close(fd[j][1]);
				j++;
			}
			error_handler();
		}	
		i++;
	}

	//Main Execution process
	i = 0;
	while (current_cmd)
	{
		vars->childid = fork();
		if (vars->childid == -1)
			error_handler();
		if (vars->childid == 0)
		{
			if (i == 0) 
			{
				close(fd[i][0]);
				first_pipe(vars, core, current_cmd, fd[i][1]);
				//close(fd[i][1]);			
			}
			else if (i == cmds - 1) 
			{
				close(fd[i - 1][1]);
				last_pipe(vars, core, current_cmd, fd[i - 1][0]);
				//close(fd[i - 1][0]);
			}
			else 
			{
				close(fd[i - 1][1]);
				close(fd[i][0]);
				if (dup2(fd[i - 1][0], STDIN_FILENO) == -1)
					error_handler_fd(fd[i - 1][0], current_cmd);
				if (dup2(fd[i][1], STDOUT_FILENO) == -1)
					error_handler_fd(fd[i][1], current_cmd);
				if (current_cmd->redir_type != 0 && current_cmd->redir_type != 10 && current_cmd->redir_type != 30)
					redirctions(current_cmd, core, vars, &fd[i][0]);
				close(fd[i - 1][0]);
				close(fd[i][1]);
			}
			closing_cmds_parent(cmds, fd);
			if (current_cmd->isbuiltin == 1)
				echo_cmd(current_cmd, core);
			else if (current_cmd->isbuiltin != 0)
				builtin_cmds(current_cmd, core);
			else if (current_cmd->args[0] && ft_strchr(current_cmd->args[0], '/'))
				absolute_path_finder(core, core->env, current_cmd->args);
			else
				path_finder(vars, core, envp, current_cmd->args, 0);
			exit(core->exit_status);
		}
		childids[i] = vars->childid;
		current_cmd = current_cmd->next;
		i++;
	}
	closing_cmds_parent(cmds, fd);
	
	//Waiting for child processes
	j = -1;
	while (++j < cmds)
	{
		waitpid(childids[j], &status, 0);
	}
	if (WIFEXITED(status))
		core->exit_status = WEXITSTATUS(status);
	
	//Removing files if its a here_doc
	j = 0;
	current_cmd = temp;
	while (cmd)
	{
		if (cmd->redir_type == 10)
		{
			unlink(current_cmd->redir);
		}
		cmd = cmd->next;
		current_cmd = current_cmd->next;
	}
	exit(core->exit_status);
}
