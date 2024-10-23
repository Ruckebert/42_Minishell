/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:14:32 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/23 13:05:59 by marsenij         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "executor/pipex.h"
# include "get_next_line/get_next_line.h"

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

//Dummy Struct for executor
typedef struct s_command
{
	char	*name;
	char	**args;
	char	*input_file;
	char	*output_file;
	int		arg_count;
	int		here_doc;
	char	*here_doc_delimiter;
	
}	t_command;

/*Core Data Struct*/
typedef struct s_data
{
	char	*user;
	char	*direct;
	char	*line;
	char	**env;
	char	**export_env;
	
}	t_data;

typedef struct s_cmdtable
{
	char	**args;
	int		has_pipe_after;
	int		redir_type;
	char	*redir;
	struct s_cmdtable *next;
	struct s_cmdtable *prev;
}	t_cmdtable;

/*Utils/Free*/
int		ft_strcmp(char *s1, char *s2);
int		len_env_var(char **argv, int j);
int		environment_export(t_data *core);

/*Builtin Functions*/
void	bubble_sort(t_data *core);
void	print_exo_env(t_data *core);
char	**new_exo_env(char **env, char **argv, int argc, int count);
int		check_dup_exo(char **env, char **argv, char **temp, int j);
int		finder(int found, int i, char **argv, char **env);
char	**unset_exo(t_data *core, char **env, int i, char **argv);
char	**unset_env(t_data *core, char **env, int i, char **argv);

typedef struct s_token
{
	char			*word;
	int				type;
	int				leading_space;
	struct s_token	*next;
	struct s_token	*prev;

}	t_token;

/*Builtins*/
void	env(t_data *core);
void	cd_com(t_data *core);
void	pwd(t_data *core);
void	export(t_data *core);
void	unset(t_data *core);
void	echo_cmd(t_data *core);

/*Environment Functions*/
char	**copy_env(char **env, t_data *core);
void	pwd_update(t_data *core);
void	envi_update(char *old_pwd, t_data *core);

/*Lexer Functions AKA Tokenizer*/

/*Parser Functions*/

/*Executor Functions*/
int		executor(t_command *cmd, t_data *core);





//all parser functions!
void parse(t_data *core, t_token * token);
void prep_nodes_for_exec(t_token *token);
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
