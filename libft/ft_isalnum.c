/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 12:35:55 by aruckenb          #+#    #+#             */
/*   Updated: 2024/04/08 12:55:07 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int chara)
{
	if ((chara >= 'a') && (chara <= 'z'))
	{
		return (1);
	}
	else if ((chara >= 'A') && (chara <= 'Z'))
	{
		return (1);
	}
	else if ((chara >= '0') && (chara <= '9'))
	{
		return (1);
	}
	else
		return (0);
}
