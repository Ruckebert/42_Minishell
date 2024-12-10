/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:26:47 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/10 09:48:26 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strcpy(char *dest, const char *src)
{
	char	*start;

	start = dest;
	while (*src != '\0')
	{
		*dest = *src;
		src++;
		dest++;
	}
	*dest = *src;
	return (start);
}

char	*ft_strdup(const char *src)
{
	char	*characterstring;
	char	*temp;
	int		len;

	characterstring = NULL;
	temp = NULL;
	if (!src)
		return (NULL);
	len = ft_strlen(src);
	len++;
	characterstring = (char *)malloc(len);
	if (characterstring == NULL)
		return (0);
	temp = ft_strcpy(characterstring, src);
	return (temp);
}
