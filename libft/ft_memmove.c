/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 14:57:59 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/10 17:04:58 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	char	*temp_src;
	char	*temp_dest;
	size_t	count;

	temp_src = (char *)src;
	temp_dest = (char *)dest;
	count = 0;
	if ((!dest && !src) || (dest == src))
		return (dest);
	if (temp_src < temp_dest)
	{
		while (n-- > 0)
			temp_dest[n] = temp_src[n];
	}
	else
	{
		while (count < n)
		{
			temp_dest[count] = temp_src[count];
			count++;
		}
	}
	return (temp_dest);
}
