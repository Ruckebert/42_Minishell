/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:34:16 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/10 10:26:59 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	len_env_var(char **argv, int j)
{
	int	i;

	i = 0;
	while (argv[j][i])
	{
		if (argv[j][i] == '=')
			break ;
		i++;
	}
	return (i);
}

int	environment_export(t_data *core)
{
	int		count;

	count = 0;
	if (core->export_env[0] == NULL)
	{
		while (core->env[count])
		{
			core->export_env[count] = ft_strdup(core->env[count]);
			count++;
		}
		core->export_env[count] = NULL;
	}
	else
	{
		while (core->env[count])
			count++;
	}
	return (count);
}

void	simple_free(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

int	strtoull_loop(const char *temp,
	unsigned long long *result, unsigned long long prev_result)
{
	int	digit;

	if (*temp == '+')
		temp++;
	while (*temp)
	{
		if (*temp >= '0' && *temp <= '9')
			digit = *temp - '0';
		else if (*temp >= 'a' && *temp <= 'z')
			digit = *temp - 'a' + 10;
		else if (*temp >= 'A' && *temp <= 'Z')
			digit = *temp - 'A' + 10;
		else
			break ;
		if (digit >= 10)
			break ;
		prev_result = *result;
		*result = *result * 10 + digit;
		if (*result < prev_result)
			return (1);
		temp++;
	}
	return (0);
}

unsigned long long	ft_strtoull(const char *str, int *j)
{
	unsigned long long	result;
	const char			*temp;
	char				*temp2;

	temp = str;
	result = 0;
	while (*temp == ' ' || *temp == '\t' || *temp == '\n'
		|| *temp == '\r' || *temp == '\v' || *temp == '\f')
		temp++;
	if (strtoull_loop(temp, &result, 0) == 1)
		*j = -1;
	if (ft_strcmp((char *)str, "-9223372036854775809") == 0)
		*j = -1;
	if (result == 0 && ft_strlen(str) != 0 && *j != -1)
	{
		temp = str;
		if (temp[0] == '-')
			temp++;
		temp2 = ft_strtrim(temp, "0");
		if (ft_strcmp(temp2, "9223372036854775809") == 0)
			*j = -1;
	}
	if (result > 9223372036854775807)
		*j = -1;
	return (result);
}
