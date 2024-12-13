/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aruckenb <aruckenb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 12:41:30 by aruckenb          #+#    #+#             */
/*   Updated: 2024/12/13 13:31:13 by aruckenb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

extern volatile sig_atomic_t g_interrupt_received;

int	main_exanpder_env(char *line, int i, t_exp *doc)
{
	int		j;
	char	*temp;

	doc->before_var = ft_substr(line, 0, i);
	temp = doc->expanded_line;
	doc->expanded_line = ft_strjoin(doc->expanded_line, doc->before_var);
	free(temp);
	free(doc->before_var);
	doc->var_start = &line[i + 1];
	j = 0;
	while (doc->var_start[j] && (ft_isalnum(doc->var_start[j])
			|| doc->var_start[j] == '_'))
		j++;
	doc->var_name = ft_substr(doc->var_start, 0, j);
	if (!doc->var_name)
		expander_freer(doc);
	doc->env_value = getenv(doc->var_name);
	free(doc->var_name);
	if (!doc->env_value)
		doc->env_value = "";
	temp = doc->expanded_line;
	doc->expanded_line = ft_strjoin(doc->expanded_line, doc->env_value);
	if (!doc->expanded_line)
		expander_freer(doc);
	i += j + 1;
	return (free(temp), i);
}

char	*expander_env(t_data *core, char *line)
{
	t_exp	doc;
	int		i;

	doc = (t_exp){0};
	i = 0;
	core->exit_status = 1;
	while (line[i])
	{
		if (line[i] == '$')
		{
			i = main_exanpder_env(line, i, &doc);
			line = &line[i];
			i = 0;
		}
		else
			i++;
	}
	if (*line)
	{
		doc.expanded_line = ft_strjoin(doc.expanded_line, line);
		if (!doc.expanded_line)
			expander_freer(&doc);
	}
	return (doc.expanded_line);
}


char	*ft_nbr_pointhex(intptr_t num)
{
	int					store;
	unsigned long		nb;
	int					count;
	char				*character;

	character = ft_calloc(21, sizeof(char *));
	nb = num;
	count = 0;
	while (nb > 0)
	{
		store = nb % 16;
		if (store < 10)
			character[count++] = 48 + store;
		else
			character[count++] = 87 + store;
		nb = nb / 16;
	}
	character[count] = '\0';
	return (character);
}

int	here_doc_main(t_exp *doc, t_cmdtable *cmd, t_data *core)
{
	if (doc->expand_line && cmd->redir_type == 30)
		free(doc->expand_line);
	if (ft_strncmp(doc->line, cmd->redir, ft_strlen(cmd->redir)) == 0
		&& ft_strlen(doc->line) == ft_strlen(cmd->redir))
	{
		free(doc->line);
		return (1);
	}
	doc->expand_line = expander_env(core, doc->line);
	if (doc->expand_line && cmd->redir_type == 10)
	{
		write(doc->tmp_fd, doc->expand_line, ft_strlen(doc->expand_line));
		free(doc->expand_line);
	}
	else
		write(doc->tmp_fd, doc->line, ft_strlen(doc->line));
	write(doc->tmp_fd, "\n", 1);
	free(doc->line);
	return (0);
}

char	*here_doc_tempfile(t_cmdtable *cmd, t_data *core, int fd)
{
	t_exp	doc;

	doc = (t_exp){0};
	setup_signal_handler(SIGINT, sig_handleINT_heredoc);
	doc.filename = ft_nbr_pointhex((intptr_t)cmd->redir);
	doc.tmp_fd = open(doc.filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (doc.tmp_fd == -1)
		error_handler_fd(fd, cmd);
	while (1)
	{
		doc.line = readline("> ");
		if (g_interrupt_received != 0)
			return (close(doc.tmp_fd), free(doc.line), doc.filename);
		if (!doc.line)
		{
			here_doc_null_msg(cmd);
			break ;
		}
		if (here_doc_main(&doc, cmd, core) == 1)
			break ;
	}
	return (close(doc.tmp_fd), doc.filename);
}
