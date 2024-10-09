/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 13:08:47 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/08 14:08:31 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	char	*temp;
	char	letter;
	int		len;
	int		count;

	count = 0;
	len = 0;
	temp = (char *)str;
	letter = (char)c;
	len = ft_strlen(temp);
	while (count <= len)
	{
		if (temp[count] == letter)
			return (&temp[count]);
		count++;
	}
	return (0);
}
