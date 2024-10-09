/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:43:40 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/10 17:10:36 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	int				ch;
	unsigned char	*temp;
	unsigned char	*temp2;

	ch = 0;
	temp = (unsigned char *)s1;
	temp2 = (unsigned char *)s2;
	while (n--)
	{
		if ((*temp < *temp2) || (*temp > *temp2))
		{
			ch = *temp - *temp2;
			return (ch);
		}
		temp++;
		temp2++;
	}
	return (0);
}
