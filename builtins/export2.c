/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 14:02:08 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/19 09:59:10 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	argv_env_loop(int *equal, int *error, char **argv, int i)
{
	int	j;
	int	first;

	j = 0;
	first = 0;
	*equal = 0;
	while (argv[i][j])
	{
		if (argv[i][j] == '=' && first == 0)
		{
			(*equal)++;
			if (j == 0)
				(*equal)++;
		}
		if ((argv[i][j] == '+' | argv[i][j] == '?' || argv[i][j] == '^'
			|| argv[i][j] == '!' || argv[i][j] == '~' || argv[i][j] == '#'
			|| argv[i][j] == '@' || argv[i][j] == '*' || argv[i][j] == '-'
			|| argv[i][j] == '.' || argv[i][j] == '{' || argv[i][j] == '}'
			|| argv[i][j] == '$') && *equal == 0)
			(*error)++;
		if (ft_isalnum(argv[i][j]) == 0 && *equal == 1)
			first++;
		j++;
	}
}

int	argv_checker(char **argv, int i, int error)
{
	int	equal;
	int	first;

	first = 0;
	if ((argv[i][0] >= '0' && argv[i][0] <= '9') || argv[i][0] == '+')
		return (exp_error_msg(argv[i]), 1);
	while (argv[i])
	{
		argv_env_loop(&equal, &error, argv, i);
		if (equal > 1 || error >= 1)
			return (exp_error_msg(argv[i]), 1);
		i++;
	}
	return (0);
}

int	check_dup(char **argv, char **temp, int k, int j)
{
	int	i;
	int	len_var;

	i = 0;
	len_var = len_env_var(argv, j);
	while (argv[i])
	{
		if (i >= j)
		{
			i++;
			continue ;
		}
		else if (ft_strncmp(argv[j], argv[i], len_var) == 0)
		{
			if (ft_strcmp(argv[j], argv[i]) != 0)
			{
				if (checker_dup(argv, temp, j, k) == -1)
					return (-1);
				else
					return (1);
			}
		}
		i++;
	}
	return (0);
}

char	**creating_new_exo(int i, char **temp, char **env, char **argv)
{
	int	found;
	int	j;

	j = 1;
	while (argv[j])
	{
		found = 0;
		found = check_dup_exo(env, argv, temp, j);
		if (found != 1)
			found = check_dup(argv, temp, i, j);
		if (found == -1)
			return (simple_free(temp), NULL);
		if (!found)
		{
			if (argv_checker(argv, 1, 0) == 1)
				return (simple_free(temp), NULL);
			temp[i] = ft_strdup(argv[j]);
			if (!temp[i])
				reverse_free(i, temp);
			i++;
		}
		j++;
	}
	temp[i] = NULL;
	return (temp);
}

char	**new_exo_env(char **env, char **argv, int argc, int count)
{
	int		i;
	char	**temp;

	temp = NULL;
	temp = malloc((argc + count) * sizeof(char *));
	if (!temp)
	{
		free_exit(address_getter(NULL));
		exit(1);
	}
	i = 0;
	while (i < argc + count)
	{
		temp[i] = NULL;
		i++;
	}
	i = 0;
	while (env[i])
	{
		temp[i] = ft_strdup(env[i]);
		if (!temp[i])
			reverse_free(i, temp);
		i++;
	}
	return (creating_new_exo(i, temp, env, argv));
}
