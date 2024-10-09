/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_hex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:57:23 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/19 11:46:20 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	write_str(char *str, int count)
{
	count--;
	while (count >= 0)
	{
		write(1, &str[count], 1);
		count--;
	}
}

static int	hexer(int lower)
{
	int	hex;

	hex = 55;
	if (lower == 0)
	{
		hex = hex + 32;
		return (hex);
	}
	else
		return (hex);
}

int	zero_hex(void)
{
	write(1, "0", 1);
	return (1);
}

int	ft_putnbr_hex(int num, int lower)
{
	int				store;
	unsigned int	nb;
	int				hex;
	int				count;
	char			character[21];

	hex = hexer(lower);
	nb = num;
	count = 0;
	if (nb == 0)
		return (zero_hex());
	while (nb > 0)
	{
		store = nb % 16;
		if (store < 10)
			character[count++] = 48 + store;
		else
			character[count++] = hex + store;
		nb = nb / 16;
	}
	character[count] = '\0';
	write_str(character, count);
	return (count);
}
