/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:24:10 by marsenij          #+#    #+#             */
/*   Updated: 2024/10/29 09:41:07 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void substitute_node_word(t_token *curr, char *new_word)
{
	t_token *next_node;
	t_token *discard;
	
	if (!curr || !curr->next) 
	    return;
	next_node = curr->next->next;
	if (next_node)
		next_node->prev = curr;
	discard = curr->next;
	curr->next = next_node;
	free(discard->word);
	free(discard);
	free(curr->word);
	curr->word = strdup(new_word);
}

void fuse_node_word(t_token *curr, char *new_word)
{
	t_token *next_node;
	t_token *discard;
	char	*oldword;
	
	oldword = curr->word;
	if (!curr || !curr->next) 
	    return;
	next_node = curr->next->next;
	if (next_node)
		next_node->prev = curr;
	discard = curr->next;
	curr->next = next_node;
	free(discard->word);
	free(discard);
	curr->word = ft_strjoin(oldword,new_word);
	free(oldword);
}

char *get_env_var(char *var, char **env)
{
	int     i;
	char    *res;
	int     valuestart;

	i = 0;
	res = NULL;
	while (env[i])
	{
		if (!strncmp(var, env[i], ft_strlen(var)) && env[i][ft_strlen(var)] == '=')
		{
			valuestart = ft_strlen(var) + 1;
			res = malloc(ft_strlen(&env[i][valuestart]) + 1);
			if (!res) 
				return (NULL);
			ft_strlcpy(res, &env[i][valuestart], ft_strlen(&env[i][valuestart]) + 1);
			return (res);
		}
		i++;
	}
	//TODO:free and return properly
	return (NULL);
}

int is_expandable(char *var, char **env)
{
    int i;

    i = 0;
    while (env[i])
    {
        if (!strncmp(var, env[i], ft_strlen(var)) && env[i][ft_strlen(var)] == '=')
        {
            return (1);
        }
        i++;
    }
    return (0);
}

void remove_next_token(t_token *current)
{
	t_token *to_remove;
	
	if (current == NULL || current->next == NULL)
		return;
	to_remove = current->next;
	current->next = to_remove->next;
	if (to_remove->next != NULL)
		to_remove->next->prev = current;
	free(to_remove->word);
	free(to_remove);
}

void expand_var(t_token *token, char **env)
{
	char	*value;
	t_token	*curr;
	
	curr = token;
	while (curr && curr->next)
	{
		if (curr->type == 8 && curr->next->leading_space == 0)
		{
			if(!strncmp(curr->next->word,"?\0", 2))
			{
					// handle $? token
			}
			else 
			{
				if (is_expandable(curr->next->word, env))
				{
					value = get_env_var(curr->next->word, env);
					substitute_node_word(curr, value);
					if (curr->leading_space == 0)
						fuse_node_word(curr->prev, value);
				}
				else
				{
					curr = curr->prev;
					remove_next_token(curr);
					remove_next_token(curr);
				}
			}
		}
		curr = curr->next;
	}
}

void remove_quotes(t_token *curr)
{
	char *temp;

	temp = malloc(ft_strlen(curr->word) - 1);

	ft_strlcpy(temp, &curr->word[1], ft_strlen(curr->word) - 1);
	free(curr->word);
	curr->word = temp;
}

char	*parse_var_name(t_token *curr)
{
	char	*start;
	int	len;
	char	*res;

	len = 0;
	start = ft_strchr(curr->word, '$');
	start++;
	if (*start != ' ')
	{
		while (start[len] != ' ' && start[len] != '\0')
			len++;
		res = malloc(len + 1);
		ft_strlcpy(res, start, len + 1);
		return(res);
	}
	else
		return (NULL);
}

void parsearound_var(t_token *curr, char **env, char *var)
{
	int	temp;
	char	*beforevar;
	char	*aftervar;
	int	i;
	char	*res;
	char	*varvalue;

	(void) env;
	(void) res;
	i = 0;
	beforevar = NULL;
	aftervar = NULL;
	while (curr->word[i] != '$')
		i++;
	beforevar = malloc (i + 1);
	ft_strlcpy(beforevar, curr->word, i + 1);
	temp = ft_strlen(&curr->word[i + ft_strlen(var) + 1]);
	aftervar = malloc (temp + 1);
	ft_strlcpy(aftervar, &curr->word[i + ft_strlen(var) + 1], temp + 1);
	if (!is_expandable(var, env))
	{
		res = ft_strjoin(beforevar, aftervar);
		free(beforevar);
		free(aftervar);
		free(curr->word);
		curr->word = res;
		if (ft_strlen(res) == 0)
		{
			curr = curr->prev;
			remove_next_token(curr);
		}
	}
	else
	{
		varvalue = get_env_var(var, env);
		res = ft_strjoin(beforevar, varvalue);
		free(beforevar);
		free(varvalue);
		beforevar = res;
		res = ft_strjoin(beforevar, aftervar);
		free(beforevar);
		free(aftervar);
		curr->word = res;
	}

}

void expand_var_in_doublequote(t_token *token, char **env)
{
	(void) env;
	t_token *curr;
	char	*var;

	curr = token;
	while (curr)
	{
		if (curr->type == 4)
		{
			remove_quotes(curr);
			if (ft_strchr(curr->word, '$') != NULL)
			{
				var = parse_var_name(curr);
				parsearound_var(curr, env, var);
			}
		}
		curr = curr->next;
	}
}

void remove_singlequotes(t_token *token)
{
	t_token	*curr;

	curr = token;
	while (curr)
	{
		if (curr->type == 5)
		{
			remove_quotes(curr);
		}
		curr = curr->next;
	}
}

void fuse_node_with_next(t_token *curr)
{
    t_token *next_node;
    t_token *discard;
    char    *oldword;

    if (!curr || !curr->next) 
        return;

    oldword = curr->word;
    next_node = curr->next->next;

    if (next_node)
        next_node->prev = curr;

    discard = curr->next;
    curr->next = next_node;

    curr->word = ft_strjoin(oldword, discard->word);
    free(oldword);
    free(discard->word);
    free(discard);
}

void	fuse_all_0space_nodes(t_token *token)
{
	t_token	*curr;

	curr = token;
	while(curr)
	{
		if ((curr->type == 4 || curr->type == 5) && curr->leading_space == 0)
		{
			fuse_node_with_next(curr);
		}
		curr = curr->next;
	}
}



t_cmdtable  *parse(t_data *core, t_token *token)
{
//	printlist_both(token);
	expand_var(token, core->env);
//	printlist(token);
	expand_var_in_doublequote(token, core->env);
//	printlist(token);
	remove_singlequotes(token);
//	printlist(token);
	fuse_all_0space_nodes(token);
//	printlist(token);
	//print_cmdtable(cmd);
	return (prep_nodes_for_exec(token));
//	handle_singlequote(token);
//	printlist(token);
//	printlist(token);
//	handle_singlequote();
	
}
