/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 10:40:54 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/13 15:57:31 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**freeaway(char **split_string, int loca)
{
	int	i;

	i = 0;
	if (split_string == NULL)
		return (NULL);
	while (i < loca)
	{
		free(split_string[i]);
		i++;
	}
	free(split_string);
	return (NULL);
}

static char	**inputing(char *temp, char **split_string, char c, int amount)
{
	int	loca;
	int	position;

	loca = 0;
	position = 0;
	if (split_string == NULL)
		return (0);
	while (*temp && (loca < amount - 1))
	{
		if (*temp != c)
		{
			while (*temp && *temp != c)
			{
				split_string[loca][position++] = *temp;
				temp++;
			}
			split_string[loca][position] = '\0';
			position = 0;
			loca++;
		}
		else
			temp++;
	}
	split_string[loca] = NULL;
	return (split_string);
}

static char	**one(void)
{
	char	**split_string;

	split_string = malloc(sizeof(char *));
	if (split_string == NULL)
		return (NULL);
	split_string[0] = NULL;
	return (split_string);
}

static char	**size_word(char *temp, char **split_string, char c, int amount)
{
	int	loca;
	int	count;

	loca = 0;
	count = 1;
	while (*temp && (loca < amount))
	{
		if (*temp != c)
		{
			while (*temp && *temp != c)
			{
				count++;
				temp++;
			}
			split_string[loca] = malloc(count * sizeof(char));
			if (split_string[loca] == NULL)
				return (freeaway(split_string, loca));
			count = 1;
			loca++;
		}
		else
			temp++;
	}
	return (split_string);
}

char	**ft_split(char const *s, char c)
{
	char	**split_string;
	int		loca;
	char	*temp;

	temp = (char *)s;
	loca = 1;
	while (*temp)
	{
		if (*temp != c)
		{
			while (*temp && *temp != c)
				temp++;
			loca++;
		}
		else
			temp++;
	}
	if (loca == 1)
		return (one());
	split_string = malloc(loca * sizeof(char *));
	if (split_string == NULL)
		return (NULL);
	split_string = size_word((char *)s, split_string, c, loca);
	split_string = inputing((char *)s, split_string, c, loca);
	return (split_string);
}
