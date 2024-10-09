/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 11:28:14 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/12 10:42:49 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	neg_size(unsigned int n, int count)
{
	int	num;

	num = n;
	count = 1;
	if (num == -2147483648)
		return (12);
	if (num < 0)
	{
		count++;
		num = num * -1;
	}
	if (n == 0)
		return (2);
	while (num > 9)
	{
		num = num / 10;
		count++;
	}
	count++;
	return (count);
}

static char	*int_str(unsigned int n, char *flip_str, int count)
{
	int	num;

	num = 0;
	count--;
	flip_str[count] = '\0';
	while (n > 0)
	{
		num = n % 10;
		n = n / 10;
		num = num + 48;
		count--;
		flip_str[count] = num;
	}
	return (flip_str);
}

static char	*zero(char *flip_str)
{
	flip_str[0] = '0';
	flip_str[1] = '\0';
	return (flip_str);
}

char	*ft_itoa(int n)
{
	int		minus;
	int		count;
	char	*flip_str;

	minus = 0;
	count = 0;
	count = neg_size((unsigned int)n, count);
	flip_str = malloc((count) * sizeof(char));
	if (flip_str == NULL)
		return (0);
	if (n == 0)
		return (zero(flip_str));
	if (n < 0)
	{
		flip_str[minus] = '-';
		n = n * -1;
		minus++;
	}
	flip_str = int_str((unsigned int)n, flip_str, count);
	return (flip_str);
}
