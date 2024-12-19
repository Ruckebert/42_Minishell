/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 09:46:50 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/19 14:20:34 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strcmp(char *s1, char *s2)
{
	while ((*s1 != '\0') && (*s2 != '\0'))
	{
		if (*s1 != *s2)
			return (*s1 - *s2);
		s1++;
		s2++;
	}
	if ((*s1 != *s2))
		return (*s1 - *s2);
	return (0);
}

void	free_exit(t_data *core)
{
	if (core->env != NULL)
		simple_free(core->env);
	if (core->export_env != NULL)
		simple_free(core->export_env);
	free(core->direct);
	free(core->line);
	if (core->cmd != NULL)
		free_cmdtable(&core->cmd);
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}

int	multi_array_counter(char **argc)
{
	int	i;

	i = 0;
	while (argc[i])
		i++;
	return (i);
}

int	checker_dup(char **argv, char **temp, int j, int k)
{
	if (check_dup_exo(temp, argv, temp, j) == 1)
		return (1);
	if (argv_checker(argv, 1, 0) == 1)
		return (-1);
	free(temp[k - j]);
	temp[k - j] = ft_strdup(argv[j]);
	if (!temp[k - j])
		export_malloc_error(address_getter(NULL), temp);
	return (1);
}

void	exit_pipe(int *fd, t_cmdtable *cmd)
{
	if (cmd->isbuiltin == 7)
	{
		close(fd[0]);
		close(fd[1]);
	}
}
