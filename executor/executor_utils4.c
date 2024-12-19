/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 14:53:00 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/19 17:43:43 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t	g_interrupt_received;

void	pipe_wait(int status, pid_t second, t_cmdtable *cmd, t_data *core)
{
	setup_signal_handler(SIGINT, sig_int_parent2);
	waitpid(second, &status, 0);
	if (WIFEXITED(status))
		core->exit_status = WEXITSTATUS(status);
	if (g_interrupt_received == 3 || g_interrupt_received == 2)
		g_interrupt_received = 0;
	free_cmdtable(&cmd);
}

static char	*ft_strcpy(char *dest, const char *src)
{
	char	*start;

	start = dest;
	while (*src != '\0')
	{
		*dest = *src;
		src++;
		dest++;
	}
	*dest = *src;
	return (start);
}

char	*ft_strdup2(const char *src)
{
	char	*characterstring;
	char	*temp;
	int		len;

	characterstring = NULL;
	temp = NULL;
	if (!src)
		return (NULL);
	len = ft_strlen(src);
	len++;
	characterstring = (char *)malloc(len);
	if (characterstring == NULL)
	{
		free_exit(address_getter(NULL));
		exit(1);
	}
	temp = ft_strcpy(characterstring, src);
	return (temp);
}

void	closing(int fd, t_data *core)
{
	if (close(fd) == -1)
	{
		ft_putstr_fd("Closing Error\n", 2);
		free_exit(core);
		exit(1);
	}
}

void	free_exit_no_cmd(t_data *core)
{
	if (core->env != NULL)
		simple_free(core->env);
	if (core->export_env != NULL)
		simple_free(core->export_env);
	free(core->direct);
	free(core->line);
	close(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
}
