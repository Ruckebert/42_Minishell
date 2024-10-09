/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 17:17:33 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/12 14:01:04 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		len;
	int		unsin_i;
	char	*strmap;
	char	*temp;

	if (s == NULL || f == NULL)
		return (0);
	len = ft_strlen(s) + 1;
	unsin_i = 0;
	strmap = malloc(len * sizeof(char));
	if (strmap == NULL)
		return (0);
	temp = (char *)s;
	while (*temp != '\0')
	{
		strmap[unsin_i] = f(unsin_i, *temp);
		unsin_i++;
		temp++;
	}
	strmap[unsin_i] = '\0';
	return (strmap);
}
