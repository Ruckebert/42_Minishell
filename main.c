/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 10:05:49 by aruckenb          #+#    #+#             */
/*   Updated: 2024/10/08 11:12:03 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_line(char *argv[])
{
	return (NULL);
}

char	**split_line(char *line, int argc)
{
	//This is the Parse
	//the parse brekas down the input into various tokens
	
	char **tokens;
	int i = 0;
	
	tokens = ft_split(line, ' ');

	//The Idea behind this While Loop is to assign the input with its corseponding token 
	//These tokens are just a placeholder
	while (i < argc)
	{
		if (tokens[i] == "WORD")
		{
		}
		else if (tokens[i] == "ASIGN_WORD")
		{
		}
		else if (tokens[i] == "RED_IN")
		{
		}
		else if (tokens[i] == "RED_OUT")
		{
		}
		else if (tokens[i] == "PIPE")
		{
		}
		else if (tokens[i] == "HERE_DOC")
		{
		}
		else if (tokens[i] == "APPEND")
		{
		}
		else if (tokens[i] == "OR")
		{
		}
		else if (tokens[i] == "AND")
		{
		}
		i++;
	}
	return (tokens);
}

int		execute_args(char **args)
{
	if (args[0] == NULL)
		return (-1);
		
	pipex();
	return (0);
}

int main(int argc, char *argv[])
{
	if (isatty(STDIN_FILENO) == 1)
	{
		//Minishell DEMO
		char *line;
		char **args;
		int status = -1;
		
		while (status == -1)
		{
			ft_printf("PeePeeShell$ ");
			line = read_line(argv); //Reads the line
			args = split_line(line, argc); //Parse
			status = execute_args(args); //Executor
			free(line);
			free(args);
			if (status >= 0)
				exit(status);
		}
	}
	else
	{
		//No Minishell
	}
	return (0);
}