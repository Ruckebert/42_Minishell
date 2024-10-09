/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:43:59 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/11 16:48:12 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*loop(char const *s, unsigned int start, size_t len, char *sub)
{
	size_t	count;

	count = 0;
	while (count <= start)
	{
		if (count == start)
		{
			count = 0;
			while (count < len)
			{
				sub[count] = *s++;
				count++;
			}
			sub[count] = '\0';
			return (sub);
		}
		count++;
		s++;
	}
	return (0);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;

	if (s == NULL)
		return (0);
	if (start >= ft_strlen(s))
	{
		sub = malloc(1 * sizeof(char));
		if (sub == NULL)
			return (NULL);
		sub[0] = '\0';
		return (sub);
	}
	else if (len > (ft_strlen(s) - start))
		len = ft_strlen(s) - start;
	sub = malloc((len + 1) * sizeof(char));
	if (sub == NULL)
		return (0);
	sub = loop(s, start, len, sub);
	return (sub);
}
