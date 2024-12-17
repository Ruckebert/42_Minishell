/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:24:10 by marsenij          #+#    #+#             */
/*   Updated: 2024/12/17 12:32:37 by marsenij         ###   ########.fr       */
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
	free_double_array(arr);
}

void printlist(t_token *head)
{
    t_token *curr;

    if (!head)
        return;

    // Print table header
    printf("%-20s | %-4s | %s\n", "WORD", "TYPE", "LEADING_SPACE");
    printf("----------------------|------|--------------\n");

    // Traverse and print each token in table format
    curr = head;
    while (curr)
    {
        // %-20s ensures the word is left-aligned with 20 characters space
        // %-4d ensures the type is left-aligned with 4 characters space
        // %d prints the leading_space value
        printf("%-20s | %-4i | %i\n", curr->word, curr->type, curr->leading_space);
        curr = curr->next;
    }
    printf("\n\n");
}

int	checkquotes(t_token *token, t_data *core)
{
	t_token	*curr;
	int		len;

	len = 0;
	curr = token;
	while (curr)
	{
		if (curr->type == 4 || curr->type == 5)
		{
			len = strlen(curr->word);
			if ((len < 2)
				|| ((curr->word[0] == '\'' && curr->word[len - 1] != '\'')
					|| (curr->word[0] == '\"' && curr->word[len - 1] != '\"')))
			{
				printf("missing quote\n");
				core->exit_status = 1;
				free_token_list(token);
				return (1);
			}
		}
		curr = curr->next;
	}
	return (0);
}


t_cmdtable	*parse(t_data *core, t_token *token)
{
//	printlist(token);
	if (checkquotes(token, core))
		return (NULL);
	handle_heredoc_delimiter(token);
	split_vars_by_sep(token);
	if (redir_before_nonexpandable(token, core) != 0)
		return (NULL);
	expand_var(token, core->env, core);
	expand_var_in_doublequote(token, core->env, core);
	remove_singlequotes(token);
	fuse_all_0space_nodes(token);
	if (first_token_directory(token, core) != 0)
		return (NULL);
	if (outredir_to_directory(token, core) != 0)
		return (NULL);
	return (prep_nodes_for_exec(token, core));
}
