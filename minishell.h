/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marsenij <marsenij@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:14:32 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/06 13:48:30 by marsenij         ###   ########.fr       */
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
	char	*filename;
	char	**filename2;
	int		file_error;
	int		fdin;
	int		fdout;
	int		childid;
	int		childid2;
}	t_var;

typedef struct s_parse_context
{
	char	*beforevar;
	char	*aftervar;
	char	*var;
}	t_parse_context;

/*Environment Functions*/
char	**copy_env(char **env, t_data *core);
void	pwd_update(t_data *core);
void	envi_update(char *old_pwd, t_data *core);

/*Utils/Free*/
int					ft_strcmp(char *s1, char *s2);
int					len_env_var(char **argv, int j);
int					environment_export(t_data *core);
unsigned long long	ft_strtoull(const char *str, int *j);
void				simple_free(char **str);

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
t_cmdtable	*return_pipe(t_cmdtable *cmd);

int		here_doc_counter(t_cmdtable *cmd);
int		pipe_checker(t_cmdtable *cmd);
void	pipe_error(int *fd);
int		executor(t_cmdtable *cmd, t_data *core);
void	error_handler(void);
void	error_handler_split(char **split);
void	free_split(char **split);
void	error_handler_fd(int fd, t_cmdtable *cmd);
void	file_input(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd);
void	file_output(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd);
void	file_append(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd);
void	here_doc(t_cmdtable *cmd, t_data *core, int fd);
char	*here_doc_tempfile(t_cmdtable *cmd, t_data *core, int fd);
void	redirctions(t_cmdtable *cmd, t_data *core, t_var *vars, int *fd);
t_cmdtable	*multi_redirections(t_cmdtable *cmd, t_data *core, t_var *vars);
void	multi_pipe(t_var *vars, t_cmdtable *cmd, t_data *core, char **envp);
void	single_pipe_exe(t_cmdtable *cmd, t_data *core, t_var *vars);
void	absolute_path_finder(t_data *core, char **envp, char **argv);
void	path_finder(t_var *vars, t_data *core, char **envp, char **argv, int i);

//all parser functions!
t_cmdtable 	*parse(t_data *core, t_token * token);
t_cmdtable 	*prep_nodes_for_exec(t_token *token);
void		ft_lstadd_next(t_token **lst, t_token *new);

//all tokenizer functions!
t_token	*tokenize(t_data *core);
int		synthax_check(t_token *token,t_data *core);
int		first_token_directory(t_token *token, t_data *core);
int		redir_before_redir(t_token *token, t_data *core);
int		redir_before_end(t_token *token, t_data *core);
int		outredir_to_directory(t_token *token, t_data *core);
//isneeded
int		is_myspace(char *c);
int		issep(char *c);
int		isquote(char *c);
int		searchquote(char *str);
int		searchsep(char *str);
t_token	*ft_lstnew(char *word);
t_token	*ft_lstlast(t_token *lst);
void	ft_lstadd_back(t_token **lst, t_token *new);
int		is_end(t_token *curr);
int		is_start(t_token *curr);
int		is_redir(t_token	*token);
void	ft_lstdelone(t_token *lst);
void	remove_empty_quotes(t_token *token);
void	setup_signal_handler(int signal, void (*handler)(int));
void	sig_handleINT_child(int signal);
void	sig_handleINT_parent(int signal);
void	sig_handleINT_parent2(int signal);
void	sig_handleINT_heredoc(int signal);
int		is_expandable(char *var, char **env);
int		redir_before_nonexpandable(t_token *token, t_data *core);
void	remove_quotes(t_token *curr);
int		is_heredoc(t_token *token);
int		pipe_before_pipe(t_token *token, t_data *core);
int		redir_before_pipe(t_token *token, t_data *core);
int		start_before_pipe(t_token *token, t_data *core);
int		pipe_before_end(t_token *token, t_data *core);
void	remove_singlequotes(t_token *token);
void	expand_var(t_token *token, char **env, t_data *core);
void	expand_var_in_doublequote(t_token *token, char **env, t_data *core);
void	split_to_token(t_token *curr);
char	*get_env_var(char *var, char **env);
char	*parse_var_name(t_token *curr);
void	substitute_node_word(t_token *curr, char *new_word);
void	parsearound_var(t_token *curr, char **env, char *var, t_data *core);
int		is_fusable(t_token *curr);
t_cmdtable	*ft_lstnew_cmd(char *redir, int type);
int		is_redir(t_token	*token);
int		is_end(t_token *curr);
int		is_start(t_token *curr);
int		is_pipe(t_token *curr);
t_cmdtable	*ft_lstlast_cmd(t_cmdtable *lst);
void	ft_lstadd_back_cmd(t_cmdtable **lst, t_cmdtable *new);
int		has_quote(t_token *curr);
void	split_vars_by_sep(t_token *token);
void	fuse_all_0space_nodes(t_token *token);
void	handle_heredoc_delimiter(t_token *token);
void	fuse_node_with_next(t_token *curr);
int		whichtoken(char c);
void	substitute_redir(t_token *curr, char str[3]);
void	make_end_token(t_token **token);
void	make_start_token(t_token **token);
void	combine_double_redirect(t_token	*token);
void	remove_empty_quotes(t_token *token);
char	*parse_var_name(t_token *curr);
void	copy_args(t_cmdtable *cmd);
t_token	*add_redir(t_cmdtable *cmd, t_token *curr);
t_token	*get_args(t_cmdtable *cmd, t_token *token);
void	find_builtins(t_cmdtable *cmd);
//for testing
void 	printlist_both(t_token *head);
void 	printCharPointerArray(char **arr);
void	printlist_type(t_token *head);
void	printlist(t_token *head);
void	free_token_list(t_token *head);
void 	print_cmdtable(t_cmdtable *cmd);
void 	free_cmdtable(t_cmdtable **head);

#endif
