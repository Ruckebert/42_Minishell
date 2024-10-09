/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:14:03 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/12 13:51:27 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	compair(char *str, char *to_find, size_t len, size_t str_len)
{
	while (*to_find != '\0')
	{
		if (*str != *to_find)
			return (0);
		if (str_len == len)
			return (0);
		str_len++;
		to_find++;
		str++;
	}
	return (1);
}

static char	*finder(char *str, char *to_find, size_t len)
{
	int		c;
	size_t	str_len;

	str_len = 0;
	while (*str != '\0' && str_len < len)
	{
		if (*str == *to_find)
		{
			c = compair(str, to_find, len, str_len);
			if (c > 0)
				return ((char *)str);
		}
		str++;
		str_len++;
	}
	return (0);
}

char	*ft_strnstr(const char *str, const char *to_find, size_t len)
{
	char	*answer;

	if (!str && to_find != NULL && len == 0)
		return (0);
	if (*to_find == '\0')
		return ((char *)str);
	answer = finder((char *)str, (char *)to_find, len);
	return (answer);
}
