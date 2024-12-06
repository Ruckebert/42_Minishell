/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:32:32 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/06 14:03:16 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	bubble_sort(t_data *core)
{
	int		swapped;
	char	*temp_str;
	int		i;

	i = 0;
	swapped = 1;
	temp_str = NULL;
	while (swapped)
	{
		swapped = 0;
		i = 0;
		while (core->export_env[i + 1])
		{
			if (ft_strcmp(core->export_env[i], core->export_env[i + 1]) > 0)
			{
				temp_str = core->export_env[i];
				core->export_env[i] = core->export_env[i + 1];
				core->export_env[i + 1] = temp_str;
				swapped = 1;
			}
			i++;
		}
	}
}

void	print_exo_env(t_data *core, int	i)
{
	int	j;
	int	has_equal;

	i = 0;
	while (core->export_env[i])
	{
		ft_printf("declare -x ");
		j = 0;
		has_equal = 0;
		while (core->export_env[i][j])
		{
			if (core->export_env[i][j] == '=')
			{
				ft_printf("%c\"", core->export_env[i][j]);
				has_equal = 1;
			}
			else
				ft_printf("%c", core->export_env[i][j]);
			j++;
		}
		if (has_equal == 1)
			ft_printf("\"");
		ft_printf("\n");
		i++;
	}
}

int	check_dup_exo(char **env, char **argv, char **temp, int j)
{
	int	k;
	int	found;
	int	var_len;
	int	env_len;

	k = 0;
	found = 0;
	var_len = 0;
	env_len = 0;
	while (env[k])
	{
		var_len = len_env_var(argv, j);
		env_len = len_env_var(temp, k);
		if (var_len == env_len)
		{
			if (ft_strncmp(temp[k], argv[j], var_len) == 0)
			{
				temp[k] = ft_strdup(argv[j]);
				found = 1;
			}
		}
		k++;
	}
	return (found);
}

void	reverse_free(int i, char **temp)
{
	while (i >= 0)
	{
		free(temp[i]);
		i--;
	}
	free(temp);
	exit(2);
}

void	exp_error_msg(char *argv)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(argv, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}
