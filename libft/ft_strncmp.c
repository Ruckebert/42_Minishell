/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 15:30:28 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/08 14:17:00 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int		ch;
	size_t	a;

	ch = 0;
	a = 0;
	while (((*s1 != '\0') && (a < n)) || ((*s2 != '\0') && (a < n)))
	{
		if ((unsigned char)*s1 < (unsigned char)*s2)
		{
			ch = (unsigned char)*s1 - (unsigned char)*s2;
			return (ch);
		}
		else if ((unsigned char)*s1 > (unsigned char)*s2)
		{
			ch = (unsigned char)*s1 - (unsigned char)*s2;
			return (ch);
		}
		s1++;
		s2++;
		a++;
	}
	return (0);
}
