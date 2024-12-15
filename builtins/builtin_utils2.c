/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 09:46:50 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/15 11:30:13 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd_update(t_data *core)
{
	int		i;
	char	*temp;

	i = 0;
	while (core->env[i])
	{
		if (ft_strncmp(core->env[i], "PWD=", 4) == 0)
		{
			temp = ft_substr(core->env[i], 0, 4);
			free(core->env[i]);
			core->env[i] = ft_strjoin(temp, core->direct);
			if (!core->env[i] || !temp)
				free_environment(core->env, i);
			free(temp);
		}
		i++;
	}
	return ;
}

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
