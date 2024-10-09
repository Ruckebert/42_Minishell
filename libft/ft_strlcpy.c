/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:13:57 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/10 17:17:34 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	char	*start;
	size_t	len;
	size_t	final_len;

	start = (char *)src;
	len = 0;
	while (*(src + len) != '\0')
		len++;
	if (!size)
		return (len);
	final_len = len;
	len = 0;
	while (--size && start[len] != '\0')
	{
		dest[len] = start[len];
		len++;
	}
	dest[len] = '\0';
	return (final_len);
}
