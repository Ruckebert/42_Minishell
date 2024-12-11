/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_around_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:24:10 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/11 15:44:17 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*build_variable_result(char *beforevar, char *varvalue, char *aftervar, t_parse_context *ctx)
{
	char	*res;
	char	*temp;

	temp = ft_strjoin(beforevar, varvalue);
	if (!temp)
		return (NULL);
	res = ft_strjoin(temp, aftervar);
	ctx->freethis = res;
	free(temp);
	return (res);
}

void	handle_special_var(t_token *curr, t_parse_context *ctx, t_data *core)
{
	char	*varvalue;
	char	*res;

	varvalue = ft_itoa(core->exit_status);
	if (!varvalue)
		return ;
	res = build_variable_result(ctx->beforevar, varvalue, ctx->aftervar, ctx);
	free(varvalue);
	free(ctx->beforevar);
	free(ctx->aftervar);
	if (res)
		curr->word = res;
}

void	handle_non_expandable(t_token *curr, t_parse_context *ctx)
{
	char	*res;

	if (curr->leading_space == 1 && curr->next->type != 9999)
		curr->next->leading_space = 1;
	res = ft_strjoin(ctx->beforevar, ctx->aftervar);
	free(ctx->beforevar);
	free(ctx->aftervar);
	free(curr->word);
	curr->word = res;
	if (ft_strlen(res) == 0)
		ft_lstdelone(curr);
}

void	handle_expandable_var(t_token *curr, t_parse_context *ctx, char **env)
{
	char	*varvalue;
	char	*res;

	varvalue = get_env_var(ctx->var, env);
	res = build_variable_result(ctx->beforevar, varvalue, ctx->aftervar, ctx);
	free(varvalue);
	free(ctx->beforevar);
	free(ctx->aftervar);
	
	if (res)
	{	
//		if (curr->word != ctx->freethis) fix this
//			free (curr->word);
		curr->word = res;
	}
}

void	parsearound_var(t_token *curr, char **env, char *var, t_data *core)
{
	t_parse_context	ctx;
	int				i;

	i = 0;
	while (curr->word[i] != '$')
		i++;
	ctx.beforevar = malloc(i + 1);
	if (!ctx.beforevar)
		return ;
	ft_strlcpy(ctx.beforevar, curr->word, i + 1);
	ctx.aftervar = malloc(ft_strlen(&curr->word[i + ft_strlen(var) + 1]) + 1);
	if (!ctx.aftervar)
		return (free(ctx.beforevar));
	ft_strlcpy(ctx.aftervar, &curr->word[i + ft_strlen(var) + 1],
		ft_strlen(&curr->word[i + ft_strlen(var) + 1]) + 1);
	ctx.var = var;
	if (ft_strcmp(var, "?") == 0)
		handle_special_var(curr, &ctx, core);
	else if (!is_expandable(var, env))
		handle_non_expandable(curr, &ctx);
	else
		handle_expandable_var(curr, &ctx, env);
	free(var);
}
