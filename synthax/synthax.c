/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synthax.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:23 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/06 09:57:07 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	redir_before_end(t_token *token, t_data *core)
{
	t_token	*curr;

	curr = token;
	if (curr->type == 9999)
		curr = curr->next;
	while (curr)
	{
		if ((curr->type == 1 | curr->type == 2
				| curr->type == 10 | curr->type == 20)
			&& (curr->next->type == 9999))
		{
			ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
			core->exit_status = 2;
			return (1);
		}
		curr = curr->next;
	}
	return (0);
}

int	start_before_pipe(t_token *token, t_data *core)
{
	t_token	*curr;

	curr = token;
	if (curr->type == 9999 && (curr->next->type == 3))
	{
		ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
		core->exit_status = 2;
		return (1);
	}
	return (0);
}

int	pipe_before_end(t_token *token, t_data *core)
{
	t_token	*curr;

	curr = token;
	if (curr->type == 9999)
		curr = curr->next;
	while (curr)
	{
		if (curr->type == 3 && (curr->next->type == 9999))
		{
			ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
			core->exit_status = 2;
			return (1);
		}
		curr = curr->next;
	}
	return (0);
}

int	pipe_before_pipe(t_token *token, t_data *core)
{
	t_token	*curr;

	curr = token;
	if (curr->type == 9999)
		curr = curr->next;
	while (curr)
	{
		if (curr->type == 3 && curr->next->type == 3)
		{
			ft_putstr_fd("syntax error near unexpected token ", 2);
			ft_putstr_fd("`", 2);
			ft_putstr_fd(curr->next->word, 2);
			ft_putstr_fd("'\n", 2);
			core->exit_status = 2;
			return (1);
		}
		curr = curr->next;
	}
	return (0);
}

int	redir_before_pipe(t_token *token, t_data *core)
{
	t_token	*curr;

	curr = token;
	if (curr->type == 9999)
		curr = curr->next;
	while (curr)
	{
		if ((curr->type == 1 | curr->type == 2
				| curr->type == 10 | curr->type == 20)
			&& curr->next->type == 3)
		{
			ft_putstr_fd("syntax error near unexpected token ", 2);
			ft_putstr_fd("`", 2);
			ft_putstr_fd(curr->next->word, 2);
			ft_putstr_fd("'\n", 2);
			core->exit_status = 2;
			return (1);
		}
		curr = curr->next;
	}
	return (0);
}
