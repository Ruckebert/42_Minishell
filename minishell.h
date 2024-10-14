/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:14:32 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/11 15:01:47 by aruckenb         ###   ########.fr       */
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

//Dummy Struct for executor
typedef struct s_command
{
	char	*name;
	char	**args;
	char	*input_file;
	char	*output_file;
	int		arg_count;
	
}	t_command;

typedef struct s_data
{
	char	*user;
	char	*direct;
	char	*line;
	char	**env;
	
}	t_data;

/*Builtins*/
void	cd_com(t_data *core);

int		executor(t_command *cmd, t_data *core);

#endif