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
	char			*op;
	int				type;
	struct s_token	*next;
	struct s_token	*prev;
	struct s_token	**head;
}	t_token;

//for split

//for testing
void printCharPointerArray(char **arr);
void printlist_type(t_token *head);
void printlist(t_token *head);
#endif
