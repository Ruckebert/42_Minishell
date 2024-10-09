/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 16:51:59 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/12 14:10:21 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	front(char *temp2, char *temp)
{
	int	len;
	int	chara;

	len = 0;
	chara = 0;
	while (temp2[chara] != '\0')
	{
		if (temp2[chara] == temp[len])
		{
			len++;
			chara = -1;
		}
		chara++;
	}
	return (len);
}

static int	back(char *temp2, char *temp)
{
	int		set;
	int		len;

	len = ft_strlen(temp);
	set = 0;
	while (temp2[set] != '\0' && len > 0)
	{
		if (temp2[set] == temp[len -1])
		{
			len--;
			set = -1;
		}
		set++;
	}
	len++;
	return (len);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*temp;
	char	*temp2;
	int		len;
	int		count;
	int		i;

	if (s1 == NULL || set == NULL)
		return (0);
	temp = (char *)s1;
	count = front((char *)set, temp);
	len = back((char *)set, temp + count);
	temp2 = malloc(len * sizeof(char));
	if (temp2 == NULL)
		return (0);
	i = 0;
	len = len + count;
	while (count < len -1)
	{
		temp2[i] = temp[count];
		i++;
		count++;
	}
	temp2[i] = '\0';
	return (temp2);
}
