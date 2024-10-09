/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 13:19:13 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/12 09:38:57 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	char const	*point;
	char		letter;

	point = NULL;
	letter = (char)c;
	while (*str != '\0')
	{
		if (*str == letter)
			point = str;
		str++;
	}
	if (*str == letter)
		return ((char *)str);
	else
		return ((char *)point);
}
