/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:14:32 by aruckenb          #+#    #+#             */
/*   Updated: 2024/11/19 12:17:52 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"

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

//Cmd Parser Table
typedef struct s_cmdtable
{
	char	**args;
	int		has_pipe_after;
	int		redir_type;
	char	*redir;
	int		isbuiltin;
	struct s_cmdtable *next;
	struct s_cmdtable *prev;

}	t_cmdtable;

/*Core Data Struct*/
typedef struct s_data
{
	char	*user;
	char	*direct;
	char	*line;
	char	**env;
	char	**export_env;
	int		exit_status;
	t_cmdtable *cmd;

}	t_data;


typedef struct s_token
{
	char			*word;
	int				type;
	int				leading_space;
	struct s_token	*next;
	struct s_token	*prev;

}	t_token;


//Piping
typedef struct s_var
{
	char	**store;
	char	**cmd;
	char	*comm;
	char	*full_comm;
	int		fdin;
	int		fdout;
	int		childid;
}	t_var;

/*Environment Functions*/
char	**copy_env(char **env, t_data *core);
void	pwd_update(t_data *core);
void	envi_update(char *old_pwd, t_data *core);

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

/*Builtins*/
void	env(t_data *core);
void	cd_oldpwd(char *old_pwd, t_data *core);
void	cd_empty(char *old_pwd, t_data *core);
void	normal_cd(char *old_pwd, t_cmdtable *cmd, t_data *core);
void	cd_com(t_cmdtable *cmd, t_data *core);
void	pwd(t_data *core);
void	export(t_cmdtable *cmd, t_data *core);
void	unset(t_cmdtable *cmd, t_data *core);
void	echo_cmd(t_cmdtable *cmd, t_data *core);
void	exit_com(t_data *core);
void	builtin_cmds(t_cmdtable *cmd, t_data *core);

/*Executor Functions*/
int		executor(t_cmdtable *cmd, t_data *core);
void	error_handler(void);
void	error_handler_split(char **split);
void	free_split(char **split);
void	error_handler_fd(int fd);
void	file_input(t_cmdtable *cmd, t_var *vars, int *fd);
void	file_output(t_cmdtable *cmd, t_var *vars, int *fd);
void	file_append(t_cmdtable *cmd, t_var *vars, int *fd);
void	here_doc(t_cmdtable *cmd, t_data *core, int fd);
char	*here_doc_tempfile(t_cmdtable *cmd, t_data *core, int fd);
void	redirctions(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd);
void	multi_pipe(t_var *vars, t_cmdtable *cmd, t_data *core, char **envp);
void	absolute_path_finder(t_data *core, char **envp, char **argv);
void	path_finder(t_var *vars, t_data *core, char **envp, char **argv, int i);

//all parser functions!
t_cmdtable *parse(t_data *core, t_token * token);
t_cmdtable *prep_nodes_for_exec(t_token *token);
void	ft_lstadd_next(t_token **lst, t_token *new);

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
int is_END(t_token *curr);
void	ft_lstdelone(t_token *lst);
//for testing
void printlist_both(t_token *head);
void printCharPointerArray(char **arr);
void	printlist_type(t_token *head);
void	printlist(t_token *head);
void	free_token_list(t_token *head);
void print_cmdtable(t_cmdtable *cmd);
void free_cmdtable(t_cmdtable **head);
#endif
