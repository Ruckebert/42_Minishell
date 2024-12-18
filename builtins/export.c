/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:32:32 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/18 12:27:42 by aruckenb         ###   ########.fr       */
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

void	print_exo_env_loop(t_data *core, int i, int has_equal)
{
	int	j;

	j = 0;
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
}

void	print_exo_env(t_data *core, int i)
{
	int	has_equal;

	i = 0;
	while (core->export_env[i])
	{
		if (core->export_env[i][0] == '_')
			i++;
		if (core->export_env[i] == NULL)
			break ;
		write(1, "declare -x ", 12);
		has_equal = 0;
		print_exo_env_loop(core, i, has_equal);
		i++;
	}
}

int	check_dup_exo(char **env, char **argv, char **temp, int j)
{
	t_int_struct	num;

	num = (t_int_struct){0};
	num.k = 0;
	num.found = 0;
	num.var_len = 0;
	num.env_len = 0;
	while (env[num.k])
	{
		num.var_len = len_env_var(argv, j);
		num.env_len = len_env_var(temp, num.k);
		if (num.var_len == num.env_len)
		{
			if (ft_strncmp(temp[num.k], argv[j], num.var_len) == 0)
			{
				if (temp[num.k])
					free(temp[num.k]);
				temp[num.k] = ft_strdup(argv[j]);
				num.found = 1;
			}
		}
		num.k++;
	}
	return (num.found);
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
