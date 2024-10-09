/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 15:58:30 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/12 14:23:43 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len;
	int		count;
	char	*new_string;

	len = 1;
	count = 0;
	if (!s1 && !s2)
		return (NULL);
	if (s1)
		len += ft_strlen(s1);
	if (s2)
		len += ft_strlen(s2);
	new_string = malloc(len * sizeof(char));
	if (new_string == NULL)
		return (0);
	len = 0;
	while (s1 && s1[len])
		new_string[count++] = s1[len++];
	len = 0;
	while (s2 && s2[len])
		new_string[count++] = s2[len++];
	new_string[count] = '\0';
	return (new_string);
}
