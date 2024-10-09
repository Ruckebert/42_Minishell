/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:27:21 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/15 14:39:35 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	overflow(const char *nptr, int minus, long num, int count)
{
	while (*nptr != '\0' && count < 20)
	{
		if (*nptr >= 48 && *nptr <= 57)
			num = num * 10 + *nptr - 48;
		else if (!(*nptr >= 48 && *nptr <= 57))
			return (-1);
		else
			return (num * minus);
		if (*nptr != '0')
			count++;
		nptr++;
	}
	if (count < 20)
		return (num * minus);
	else if (minus == -1)
		return (0);
	else
		return (-1);
}

int	ft_atoi(const char *nptr)
{
	long	num;
	int		minus;
	int		count;

	nptr = (char *)nptr;
	num = 0;
	minus = 1;
	count = 0;
	while ((*nptr > 8 && *nptr < 14) || (*nptr == ' '))
		nptr++;
	if ((*nptr == '+') || (*nptr == '-'))
	{
		if (*nptr == '-')
			minus = minus * -1;
		nptr++;
	}
	return (overflow(nptr, minus, num, count));
}
