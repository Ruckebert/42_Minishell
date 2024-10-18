/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:14:32 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/18 11:30:57 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "pipex.h"
// All allowed functions are in these headers 
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <curses.h>

typedef struct s_data
{
	char	*user;
	char	*direct;
	char	*line;
	char	**env;
	
}	t_data;

typedef struct s_token
{
	char			*word;
	int				type;
	int				leading_space;
	struct s_token	*next;
	struct s_token	*prev;

}	t_token;

/*Builtins*/
void	cd_com(t_data *core);

//all parser functions!
void parse(t_data *core, char **env, t_token * token);

//all tokenizer functions!
t_token *tokenize(t_data *core);
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
void printlist_both(t_token *head);
void printCharPointerArray(char **arr);
void printlist_type(t_token *head);
void printlist(t_token *head);
void	free_token_list(t_token *head);
#endif
