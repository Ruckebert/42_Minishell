/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_pointhex.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:57:23 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/23 13:15:26 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	write_str(char *str, int count)
{
	count--;
	write(1, "0x", 2);
	while (count >= 0)
	{
		write(1, &str[count], 1);
		count--;
	}
}

int	zero_point(void)
{
	write(1, "(nil)", 5);
	return (5);
}

int	ft_putnbr_pointhex(intptr_t num)
{
	int					store;
	unsigned long		nb;
	int					count;
	char				character[21];

	nb = num;
	count = 0;
	if (nb == 0)
		return (zero_point());
	while (nb > 0)
	{
		store = nb % 16;
		if (store < 10)
			character[count++] = 48 + store;
		else
			character[count++] = 87 + store;
		nb = nb / 16;
	}
	character[count] = '\0';
	write_str(character, count);
	return (count + 2);
}
