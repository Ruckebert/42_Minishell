/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:24:10 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/06 13:34:52 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_env_var(char *var, char **env)
{
	int		i;
	char	*res;
	int		valuestart;

	i = 0;
	res = NULL;
	while (env[i])
	{
		if (!ft_strncmp(var, env[i], ft_strlen(var))
			&& env[i][ft_strlen(var)] == '=')
		{
			valuestart = ft_strlen(var) + 1;
			res = malloc(ft_strlen(&env[i][valuestart]) + 1);
			if (!res)
				return (NULL);
			ft_strlcpy(res, &env[i][valuestart],
				ft_strlen(&env[i][valuestart]) + 1);
			return (res);
		}
		i++;
	}
	return (NULL);
}

void	remove_next_token(t_token *current)
{
	t_token	*to_remove;

	if (current == NULL || current->next == NULL)
		return ;
	to_remove = current->next;
	current->next = to_remove->next;
	if (to_remove->next != NULL)
		to_remove->next->prev = current;
	free(to_remove->word);
	free(to_remove);
}

void	split_to_token(t_token *curr)
{
	t_token	*newtoken;
	char	**arr;
	int		i;
	int		temp_leading_space;

	temp_leading_space = 7777;
	i = 0;
	arr = ft_split(curr->word, ' ');
	curr = curr->prev;
	temp_leading_space = curr->next->leading_space;
	ft_lstdelone(curr->next);
	while (arr[i] != NULL)
	{
		newtoken = ft_lstnew(arr[i]);
		ft_lstadd_next(&curr, newtoken);
		newtoken->type = 0;
		if (i == 0)
			newtoken->leading_space = temp_leading_space;
		else
			newtoken->leading_space = 1;
		i++;
		curr = curr->next;
	}
	free (arr);
}

t_cmdtable  *parse(t_data *core, t_token *token)
{
	printf("\033[0;31m 1 \033[0m\n");
	printlist(token);

	handle_heredoc_delimiter(token);
//	printf("\033[0;31m 2 \033[0m\n");
//	printlist(token);
	
	split_vars_by_sep(token);
//	printf("\033[0;31m 2b \033[0m\n");
//	printlist(token);

	if (redir_before_nonexpandable(token, core) != 0)
		return (NULL);

	expand_var(token, core->env, core);
//	printf("\033[0;31m 3 \033[0m\n");
//	printlist(token);

	expand_var_in_doublequote(token, core->env, core);
//	printf("\033[0;31m 4 \033[0m\n");
//	printlist(token);

	remove_singlequotes(token);
//	printf("\033[0;31m 5 \033[0m\n");
//	printlist(token);

	fuse_all_0space_nodes(token);
//	printf("\033[0;31m 6 \033[0m\n");
//	printlist(token);
	if (first_token_directory(token, core) != 0)
		return(NULL);
	if (outredir_to_directory(token, core) != 0)
		return(NULL);
	printf("\033[0;31mAFTER parse.c\033[0m\n");
	printlist(token);
	return (prep_nodes_for_exec(token));
//	handle_singlequote(token); remove this function

//	printlist(token);
//	handle_singlequote();
	
}
