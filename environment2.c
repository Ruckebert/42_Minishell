/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 11:07:58 by aruckenb          #+#    #+#             */
/*   Updated: 2025/01/05 09:28:34 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	dup_pwd_env(t_data *core, char **temp, char *argv)
{
	int	i;

	i = 0;
	if (core->empty_cd == 2 || core->empty_cd == 0)
		argv = ft_strjoin("PWD=", core->direct);
	while (core->env[i])
	{
		temp[i] = ft_strdup(core->env[i]);
		if (!temp[i])
			reverse_free(i, temp);
		i++;
	}
	temp[i] = ft_strdup2(argv);
	if (core->empty_cd == 0)
	{
		i++;
		free(argv);
		argv = ft_strjoin("OLDPWD=", core->direct);
		temp[i] = ft_strdup2(argv);
	}
	temp[++i] = NULL;
	simple_free(core->env);
	free(argv);
	core->env = temp;
}

void	temp_exo_env(t_data *core, int num)
{
	char	**temp_exo;

	temp_exo = NULL;
	if (core->empty_cd == 2 || core->empty_cd == 1)
	{
		temp_exo = ft_calloc(num + 2, sizeof(char *));
		if (!temp_exo)
			export_malloc_error(core, NULL);
		simple_free(core->export_env);
		core->export_env = temp_exo;
		core->export_env[0] = NULL;
	}
	else if (core->empty_cd == 3)
	{
		temp_exo = ft_calloc(num + 3, sizeof(char *));
		if (!temp_exo)
			export_malloc_error(core, NULL);
		simple_free(core->export_env);
		core->export_env = temp_exo;
		core->export_env[0] = NULL;
	}
}

void	other_pwd_type(int num, t_data *core)
{
	char	**temp;
	char	*argv;

	temp = NULL;
	argv = NULL;
	if (core->empty_cd == 2)
	{
		temp = ft_calloc(num + 2, sizeof(char *));
		if (!temp)
			export_malloc_error(core, NULL);
		dup_pwd_env(core, temp, argv);
	}
	else if (core->empty_cd == 0)
	{
		temp = ft_calloc(num + 3, sizeof(char *));
		if (!temp)
			export_malloc_error(core, NULL);
		dup_pwd_env(core, temp, argv);
	}
}

void	create_pwd(t_data *core, char *old_pwd)
{
	char	*argv;
	char	**temp;
	int		num;

	num = 0;
	temp = NULL;
	while (core->env[num])
		num++;
	temp_exo_env(core, num);
	if (core->empty_cd == 1)
	{
		temp = ft_calloc(num + 2, sizeof(char *));
		if (!temp)
			export_malloc_error(core, NULL);
		argv = ft_strjoin("OLDPWD=", old_pwd);
		dup_pwd_env(core, temp, argv);
	}
	other_pwd_type(num, core);
}

void	pwd_checker(char **temp, char *old_pwd, t_data *core, int i)
{
	if (ft_strncmp(core->env[i], "PWD=", 4) == 0)
	{
		(*temp) = ft_substr(core->env[i], 0, 4);
		if (!(*temp))
			export_malloc_error(core, NULL);
		free(core->env[i]);
		core->env[i] = ft_strjoin(*temp, core->direct);
		if (!core->env)
			export_malloc_error(core, NULL);
		free(*temp);
		core->empty_cd++;
	}
	else if (ft_strncmp(core->env[i], "OLDPWD=", 7) == 0)
	{
		(*temp) = ft_substr(core->env[i], 0, 7);
		if (!(*temp))
			export_malloc_error(core, NULL);
		free(core->env[i]);
		core->env[i] = ft_strjoin(*temp, old_pwd);
		if (!core->env)
			export_malloc_error(core, NULL);
		free(*temp);
		core->empty_cd += 2;
	}
}
