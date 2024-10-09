/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 13:12:42 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/24 10:21:53 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen_prt(const char *str)
{
	int	len;

	len = 0;
	while (*str != '\0')
	{
		len++;
		str++;
	}
	return (len);
}

char	*ft_strchr_prt(const char *str, int c)
{
	char	*temp;
	char	letter;
	int		len;
	int		count;

	count = 0;
	len = 0;
	temp = (char *)str;
	letter = (char)c;
	len = ft_strlen_prt(temp);
	while (count <= len)
	{
		if (temp[count] == letter)
			return (&temp[count]);
		count++;
	}
	return (0);
}

int	funk(va_list nex_arg, char s)
{
	if (s == 'c')
		return (ft_putchar_print(va_arg(nex_arg, int)));
	else if (s == 's')
		return (ft_putstr_print(va_arg(nex_arg, char *)));
	else if (s == 'p')
		return (ft_putnbr_pointhex(va_arg(nex_arg, intptr_t)));
	else if ((s == 'd') || (s == 'i'))
		return (ft_putnbr_count(va_arg(nex_arg, long int)));
	else if (s == 'u')
		return (ft_putnbr_unsigned(va_arg(nex_arg, unsigned int)));
	else if (s == 'x')
		return (ft_putnbr_hex(va_arg(nex_arg, int), 0));
	else if (s == 'X')
		return (ft_putnbr_hex(va_arg(nex_arg, int), 1));
	else if (s == '%')
		return (ft_putchar_print('%'));
	else
		return (-1);
}

int	ft_printf(const char *s, ...)
{
	int		count;
	va_list	nex_arg;

	count = 0;
	if (s == NULL)
		return (-1);
	va_start(nex_arg, s);
	while (*s)
	{
		if (*s == '%' && ft_strchr_prt(" cspiduxX%", *(s + 1)))
		{
			if (*(s + 1) == '\0')
				return (-1);
			count += funk(nex_arg, *(s + 1));
			s++;
		}
		else
			count += ft_putchar_print(*s);
		s++;
	}
	va_end(nex_arg);
	return (count);
}
