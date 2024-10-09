/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 14:31:10 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/10 17:16:29 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*temp;
	char	*temp2;
	size_t	count;

	temp = (char *)dest;
	temp2 = (char *)src;
	count = 0;
	if (!dest && !src)
		return (dest);
	while (count < n)
	{
		temp[count] = temp2[count];
		count++;
	}
	return (temp);
}
