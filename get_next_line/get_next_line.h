/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 09:32:24 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/23 13:30:09 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 20000000000
# endif

char	*get_next_line(int fd);
size_t	ft_strlen(const char *str);
char	*ft_strchr(const char *str, int c);
//char	*ft_strjoin(char const *s1, char const *s2);
//char	*ft_strdup(const char *src);

#endif
