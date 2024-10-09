/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:57:23 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/08 15:47:57 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	checker(int nb, int fd)
{
	if (nb == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return (0);
	}
	if (nb < 0)
	{
		nb = nb * -1;
		write(fd, "-", 1);
		return (nb);
	}
	if (nb == 2147483647)
	{
		write(fd, "2147483647", 10);
		return (0);
	}
	if (nb == 0)
	{
		nb = 48;
		write(fd, &nb, 1);
		return (0);
	}
	return (nb);
}

static void	write_str(char *str, int count, int fd)
{
	while (count >= 0)
	{
		write(fd, &str[count], 1);
		count--;
	}
}

void	ft_putnbr_fd(int nb, int fd)
{
	int		store;
	int		count;
	char	character[20];

	count = 0;
	nb = checker(nb, fd);
	if (count == nb)
		return ;
	while (nb > 0)
	{
		store = nb % 10;
		nb = nb / 10;
		character[count] = store + 48;
		count++;
	}
	character[count] = '\0';
	count--;
	write_str(character, count, fd);
}
