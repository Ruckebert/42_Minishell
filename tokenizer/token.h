/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 13:29:32 by marsenij          #+#    #+#             */
/*   Updated: 2024/10/11 15:06:27 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_H
# define TOKEN_H

# define TOKEN_QUOTED 10
# define TOKEN_WORD 0

typedef struct s_token
{
	char			*word;
	int				type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

//isneeded
int	is_myspace(char *c);
int	issep(char *c);
int	isquote(char *c);
int	searchquote(char *str);
int	searchsep(char *str);
t_token	*ft_lstnew(char *word);
t_token	*ft_lstlast(t_token *lst);
void	ft_lstadd_back(t_token **lst, t_token *new);
//for testing
void printCharPointerArray(char **arr);
void printlist_type(t_token *head);
void printlist(t_token *head);
void	free_token_list(t_token *head);
#endif
