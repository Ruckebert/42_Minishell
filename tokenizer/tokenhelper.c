/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenhelper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:23 by marsenij          #+#    #+#             */
/*   Updated: 2024/11/29 12:52:37 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	isquote(char *c)
{
	if (*c == '\'')
		return (2);
	if (*c == '"')
		return (1);
	return (0);
}

int	issep(char *c)
{
	if (*c == '<' || *c == '>' ||  *c == '|' || *c == '=' || *c == '$' || *c == '?')
		return (1);
	return (0);
}

int	searchquote(char *str)
{
	int i;
	char quote_type = *str;

	i = 1;
	while (str[i] != '\0' && str[i] != quote_type)
		i++;
	if (str[i] == quote_type)
		i++;
	return (i);
}

int	searchsep(char *str)
{
	int i;

	i = 0;
	while(!(issep(&str[i])) && str[i] != '\0' && str[i] != ' ' && str[i] != '\t')
	{
		if(isquote(&str[i]))
			return(i);
		i++;
	}
	return (i);
}

int	is_myspace(char *c)
{
	if(*c == ' ' || *c == '\t')
		return (1);
	return (0);
}

