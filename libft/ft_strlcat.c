/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:21:48 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/12 13:49:14 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	char	*temp;
	size_t	len;
	size_t	total;

	if ((dest == NULL) && (src != NULL) && (size == 0))
		return (0);
	temp = (char *)src;
	len = 0;
	total = 0;
	while (dest[len] != '\0')
		len++;
	while (src[total] != '\0')
		total++;
	if (len < size)
		total = len + total;
	else
		return (size + total);
	while (*temp != '\0' && (len + 1) < size)
	{
		dest[len] = *temp++;
		len++;
	}
	dest[len] = '\0';
	return (total);
}
