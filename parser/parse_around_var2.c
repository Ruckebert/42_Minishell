/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_around_var2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:24:10 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/18 17:55:44 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_string_to_double_array(char ***array,
	int *num_elements, char *new_string)
{
	char	**new_array;
	int		i;

	i = 0;
	new_array = malloc((*num_elements + 2) * sizeof(char *));
	if (new_array == NULL)
		free_all();
	while (i < *num_elements)
	{
		new_array[i] = (*array)[i];
		i++;
	}
	new_array[*num_elements] = new_string;
	new_array[*num_elements + 1] = NULL;
	free(*array);
	*array = new_array;
	(*num_elements)++;
}

int	is_string_in_array(char **array, char *str)
{
	int	i;

	i = 0;
	if (array == NULL)
		return (0);
	while (array[i])
	{
		if (array[i] == str)
			return (1);
		i++;
	}
	return (0);
}
