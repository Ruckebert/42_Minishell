/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 08:24:10 by marsenij          #+#    #+#             */
/*   Updated: 2024/11/27 14:15:00 by aruckenb         ###   ########.fr       */
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
	int		firstspace_len;
	char	*res;

	firstspace_len = 0;
	res = NULL;
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
	if (ft_strchr(new_word, ' ') != NULL)
	{
		res = ft_strchr(new_word, ' ');
		firstspace_len = (res - new_word);
		ft_strlcpy(res, new_word, firstspace_len + 1);
		curr->word = ft_strjoin(oldword, res);
	}	
	else
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

void remove_quotes(t_token *curr)
{
	char *temp;

	if (curr->word[0] == '\'' || curr->word[0] == '\"')
	{
		temp = malloc(ft_strlen(curr->word) - 1);

		ft_strlcpy(temp, &curr->word[1], ft_strlen(curr->word) - 1);
		free(curr->word);
		curr->word = temp;
	}
}

void split_to_token(t_token *curr)
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
	while(arr[i]!= NULL)
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

int	is_heredoc(t_token *token)
{
	t_token *curr;
	curr = token;
	
	if (curr->type == 10)
		return (1);
	return (0);
}

void expand_var(t_token *token, char **env, t_data *core)
{
	char	*value;
	t_token	*curr;
	
	curr = token;
	while (curr && curr->next)
	{
		if (curr->type == 8 && curr->next->leading_space == 0)
		{
			if(ft_isalpha(curr->next->word[0]) || !ft_strcmp(curr->next->word,"_") || !ft_strcmp(curr->next->word,"?"))
			{
			if(!ft_strcmp(curr->next->word,"?"))
			{
				ft_lstdelone(curr->next);
				free(curr->word);
				curr->word = ft_itoa(core->exit_status);
			}
			else 
			{
				if (is_expandable(curr->next->word, env))
				{
					value = get_env_var(curr->next->word, env);
					substitute_node_word(curr, value);		
					if (ft_strchr(curr->word, ' ') != NULL)
						split_to_token(curr);
				}
				else
				{
					ft_lstdelone(curr->next);
					curr = curr->prev;
					ft_lstdelone(curr->next);
					/*
					remove_next_token(curr);
					free(curr->word);
					curr->word = malloc (1);
					curr->word[0] = '\0';
					*/
				}
			}
			}
		}
		curr = curr->next;
	}
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
		while (start[len] != ' ' && start[len] != '\0' && start[len] != '\'' && start[len] != '/')
			len++;
		res = malloc(len + 1);
		ft_strlcpy(res, start, len + 1);
		return(res);
	}
	else
		return (NULL);
}

void parsearound_var(t_token *curr, char **env, char *var, t_data *core)
{
	int		temp;
	char	*beforevar;
	char	*aftervar;
	int		i;
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
	if (ft_strcmp(var,"?") == 0)
	{
		varvalue = ft_itoa(core->exit_status);
		res = ft_strjoin(beforevar, varvalue);
		free(beforevar);
		free(varvalue);
		beforevar = res;
		res = ft_strjoin(beforevar, aftervar);
		free(beforevar);
		free(aftervar);
		curr->word = res;
		free(var);
	}
	else if (!is_expandable(var, env))
	{
		curr->next->leading_space = curr->leading_space;
		res = ft_strjoin(beforevar, aftervar);
		free(beforevar);
		free(aftervar);
		free(curr->word);
		curr->word = res;
		if (ft_strlen(res) == 0)
		{
			ft_lstdelone(curr);
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

void expand_var_in_doublequote(t_token *token, char **env, t_data *core)
{
	t_token *curr;
	char	*var;
	char	*temp;
	
	curr = token;
	while (curr)
	{
		if (curr->type == 4)
		{
			remove_quotes(curr);
			temp = ft_strchr(curr->word, '$');
			while (temp != NULL)
			{
				if(*(temp+1) != ' ' && (*(temp+1) == '_' || ft_isalpha(*(temp+1)) || *(temp+1) == '?'))
				{
					if (*(temp+1) == '?')
					{
						var = strdup("?"); 
					}
					else
					{
						var = parse_var_name(curr);
					}
					parsearound_var(curr, env, var, core);
					
				}
				temp = temp + 1;
				temp = ft_strchr(temp, '$');
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
		if ((curr->type == 8 || curr->type == 4 || curr->type == 5 || curr->type == 7 || curr->type == 9) && curr->leading_space == 0 && curr->prev->type !=9999 )
		{
			if (!is_redir(curr) && !is_redir(curr->prev))
			{
				curr->prev->type = curr->type;
				fuse_node_with_next(curr->prev);
				curr = curr->prev;
			}
		}
		if ((curr->type == 8 || curr->type == 4 || curr->type == 5 || curr->type == 7 || curr->type == 9) && curr->next->type !=9999 && curr->next->leading_space == 0)
		{
			if (!is_redir(curr) && !is_redir(curr->next))
			{
				
				fuse_node_with_next(curr);
				curr = curr->prev;
			}
		}
		curr = curr->next;
	}
}

int has_quote(t_token *curr)
{

	if ((curr->word[0] == '\'' && curr->word[ft_strlen(curr->word) - 1] == '\'')
		||(curr->word[0] == '\"' && curr->word[ft_strlen(curr->word) - 1] == '\"'))
		return (1);
	return (0);
}

void handle_heredoc_delimiter(t_token *token)
{
	t_token	*curr;

	curr = token;
	while(curr)
	{
		if (!is_START(curr) && is_heredoc(curr->prev))
		{
			if (curr->next->leading_space != 0 && has_quote(curr))
			{
				remove_quotes(curr);
				curr->type = 30;
			}
			while(curr->next->leading_space == 0 && !is_redir(curr->next) && curr->next->type != 3)
			{
				if (has_quote(curr))
				{
					remove_quotes(curr);
					curr->type = 30;
				}
				if (has_quote(curr->next))
				{
					remove_quotes(curr->next);
					curr->type = 30;
				}
				if (curr->next->type == 9)
					curr->type = 30;
				fuse_node_with_next(curr);
			}
		}
		curr = curr->next;
	}
}

void	remove_empty_quotes(t_token *token)
{
	t_token	*curr;

	curr = token;
	while(curr)
	{
		if((curr->type == 5 || curr->type == 4) && ft_strlen(curr->word) == 2)
		{
			free(curr->word);
			curr->word = ft_strdup("");
			curr->type = 9;
		}
		curr = curr->next;
	}

}

int has_var_sep(char *str)
{
	int	i;
	
	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_isalnum(str[i]))
			return (i);
		i++;
	}
	return (0);
}

char which_var_sep(char *str)
{
	int	i;
	
	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_isalnum(str[i]))
			return (str[i]);
		i++;
	}
	return ('\0');
}

void split_vars_by_sep(t_token *token)
{
	t_token *curr;
	int		i;
	t_token *newtoken;
	char *str1;
	char *str2;


	i = 0;
	curr = token;
	while(curr)
	{
		if (curr->type == 8 && has_var_sep(curr->next->word) && ft_strlen(curr->next->word) != 1)
		{
			
			i = has_var_sep(curr->next->word);
			str1 = malloc (i + 1);
			str2 = malloc (ft_strlen(&curr->next->word[i]) + 1);
			ft_strlcpy(str1, curr->next->word, i + 1);
			ft_strlcpy(str2, &curr->next->word[i], ft_strlen(&curr->next->word[i]) + 1);
			free (curr->next->word);
			curr->next->word = str1;
			curr = curr->next;
			newtoken = ft_lstnew(str2);
			ft_lstadd_next(&curr, newtoken);
		}
		curr = curr->next;
	}
}

/*
void split_vars_by_sep(t_token *token)
{
	t_token *curr;
	char **arr;
	int		i;
	t_token *newtoken;
	char *temp;
	char *sep;

	i = 0;
	curr = token;
	while(curr)
	{
		
		if (curr->type == 8 && has_var_sep(curr->next->word) && ft_strlen(curr->next->word) != 1)
		{
			sep = malloc(2);
			sep[1] = '\0';
			sep[0] = which_var_sep(curr->next->word);
			arr = ft_split(curr->next->word, sep[0]);
			free(curr->next->word);
			curr->next->word = arr[i];
			curr = curr->next;
			i++;
			while (arr[i])
			{
				temp = ft_strjoin(sep, arr[i]);
				free(arr[i]);
				newtoken = ft_lstnew(temp);
				ft_lstadd_next(&curr, newtoken);
				curr = curr->next;
				i++;
			}
			free (arr);
			free (sep);
		}
		curr = curr->next;
	}
}*/

t_cmdtable  *parse(t_data *core, t_token *token)
{
//	printf("\033[0;31m 1 \033[0m\n");
//	printlist(token);

	handle_heredoc_delimiter(token);
//	printf("\033[0;31m 2 \033[0m\n");
//	printlist(token);
	
	split_vars_by_sep(token);
//	printf("\033[0;31m 2b \033[0m\n");
//	printlist(token);
	
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
	//print_cmdtable(cmd);
//	printf("\033[0;31mAFTER parse.c\033[0m\n");
//	printlist(token);
	return (prep_nodes_for_exec(token));
//	handle_singlequote(token); remove this function

//	printlist(token);
//	handle_singlequote();
	
}
