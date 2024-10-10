/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 10:15:07 by aruckenb          #+#    #+#             */
/*   Updated: 2024/05/17 10:16:47 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"

# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>

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

void	error_handler(void);
void	error_handler_split(char **split);
void	free_split(char **split);
void	path_finder_error(char **store, char **cmd);
void	error_handler_fd(int fd);

#endif