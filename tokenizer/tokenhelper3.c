/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenhelper3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:23 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/19 15:07:50 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_leading_space(t_token *newtoken, t_token *token,
	t_data *core, int oldpos)
{
	if (token->next != NULL && core->line[oldpos - 1] == ' ')
		newtoken->leading_space = 1;
	else
		newtoken->leading_space = 0;
}
