/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synthax.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:23 by marsenij          #+#    #+#             */
/*   Updated: 2024/11/28 12:25:19 by marsenij         ###   ########.fr       */
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

int	redir_before_redir(t_token *token, t_data *core)
{
	t_token	*curr;

	curr = token;
	if (curr->type == 9999)
		curr = curr->next;
	while (curr)
	{
		if ((curr->type == 1 | curr->type == 2
				| curr->type == 10 | curr->type == 20)
			&& (curr->next->type == 1 | curr->next->type == 2
				| curr->next->type == 10 | curr->next->type == 20))
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

int	first_token_directory(t_token *token, t_data *core)
{
	struct stat	sb;
	t_token		*curr;

	curr = token;
	if (curr->type == 9999)
		curr = curr->next;
	if (stat(curr->word, &sb) == 0)
	{
		if (S_ISDIR(sb.st_mode))
		{
			if (ft_strcmp(".", curr->word) == 0
				|| ft_strcmp("..", curr->word) == 0)
				return (0);
			ft_putstr_fd(curr->word, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			core->exit_status = 126;
			return (1);
		}
		else
			return (0);
	}
	return (0);
}

int	outredir_to_directory(t_token *token, t_data *core)
{
	struct stat	sb;
	t_token		*curr;

	curr = token;
	if (curr->type == 9999)
		curr = curr->next;
	if (curr->type == 2 || curr->type == 20)
	{
		curr = curr->next;
		if (stat(curr->word, &sb) == 0)
		{
			if (S_ISDIR(sb.st_mode))
			{
				if (ft_strcmp(".", curr->word) == 0
					|| ft_strcmp("..", curr->word) == 0)
					return (0);
				ft_putstr_fd(curr->word, 2);
				ft_putstr_fd(": Is a directory\n", 2);
				core->exit_status = 1;
				return (1);
			}
			else
				return (0);
		}
	}
	return (0);
}

int	synthax_check(t_token *token, t_data *core)
{
	int	check;

	check = 0;
	check += redir_before_redir(token, core);
	if (check == 0)
		check += pipe_before_pipe(token, core);
	if (check == 0)
		check += redir_before_end(token, core);
	if (check == 0)
		check += pipe_before_end(token, core);
	if (check == 0)
		check += start_before_pipe(token, core);
	if (check == 0)
		check += redir_before_pipe(token, core);
	return (check);
}
