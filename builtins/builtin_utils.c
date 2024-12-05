/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 11:34:16 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/05 12:47:05 by aruckenb         ###   ########.fr       */
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

int	len_env_var(char **argv, int j)
{
	int i;
	
	i = 0;
	while (argv[j][i])
	{
		if (argv[j][i] == '=')
			break ;
		i++;
	}
	return (i);
}

int		environment_export(t_data *core)
{
	int count;

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
	int i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

unsigned long long ft_strtoull(const char *str, int *j)
{
	unsigned long long result = 0;
	unsigned long long prev_result = 0;
	int digit;
	const char *temp = str;

	while (*temp == ' ' || *temp == '\t' || *temp == '\n' || *temp == '\r' || *temp == '\v' || *temp == '\f')
        temp++;
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
            break;
        if (digit >= 10)
            break;
        prev_result = result;
        result = result * 10 + digit;
        if (result < prev_result)
		{
            *j = -1;
            break;
        }
        temp++;
    }
	if (ft_strcmp((char *)str, "-9223372036854775809") == 0)
		*j = -1;
	if (result == 0 && ft_strlen(str) != 0 && *j != -1)
	{
		char *temp2;
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
