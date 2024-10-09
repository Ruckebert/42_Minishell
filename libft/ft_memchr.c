/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:31:56 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/11 09:34:40 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	char	*temp;
	char	letter;

	temp = (char *)s;
	letter = (char)c;
	while (n--)
	{
		if (*temp == letter)
			return (temp);
		temp++;
	}
	return (0);
}
