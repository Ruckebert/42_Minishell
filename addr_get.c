/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addr_get.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:58:57 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/19 17:42:09 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_parse_context	*address_getter_ctx(t_parse_context *ctx)
{
	static t_parse_context	*new_ctx;

	if (ctx == NULL)
		return (new_ctx);
	else
	{
		new_ctx = ctx;
		return (new_ctx);
	}
}

t_cmdtable	**address_getter_cmd(t_cmdtable **cmd)
{
	static t_cmdtable	**new_cmd;

	if (cmd == NULL)
		return (new_cmd);
	else
	{
		new_cmd = cmd;
		return (new_cmd);
	}
}

t_token	*address_getter_token(t_token *token)
{
	static t_token	*new_token;

	if (token == NULL)
		return (new_token);
	else
	{
		new_token = token;
		return (new_token);
	}
}

t_data	*address_getter(t_data *core)
{
	static t_data	*new_core;

	if (core == NULL)
		return (new_core);
	else
	{
		new_core = core;
		return (new_core);
	}
}
