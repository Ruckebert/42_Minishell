/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:51:57 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/19 11:45:46 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr_print(char *s)
{
	int		count;
	char	*n;

	count = 0;
	n = "(null)";
	if (s == NULL)
	{
		write(1, n, 6);
		return (6);
	}
	while (*s != '\0')
	{
		write(1, s, 1);
		s++;
		count++;
	}
	return (count);
}
