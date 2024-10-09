/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 16:28:35 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/12 13:55:17 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmeb, size_t size)
{
	void	*ptr;

	if (size != 0 && nmeb != ((nmeb * size) / size))
		return (NULL);
	ptr = malloc(nmeb * size);
	if (ptr == NULL)
		return (0);
	ft_bzero(ptr, (nmeb * size));
	return (ptr);
}
