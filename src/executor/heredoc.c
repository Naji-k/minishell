/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/05 18:56:59 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/06/05 18:56:59 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	is_heredoc(t_commands **cmd, t_tools *tools)
{
	t_commands	*s_cmd;
	t_token		*heredoc;

	s_cmd = (*cmd);
	while (s_cmd)
	{
		if (s_cmd->redirections && s_cmd->redirections->type == HEREDOC)
		{
			heredoc = s_cmd->redirections;
			while (heredoc && heredoc->type == HEREDOC)
			{
				dprintf(2, "Create heredoc named as %s\n", heredoc->cmd);
				if (create_heredoc(heredoc, s_cmd, tools) == -1)
					return (ERROR);
				heredoc = heredoc->next;
			}
		}
		s_cmd = s_cmd->next;
	}
	return (SUCCESS);
}

char	*get_expanded_arg(char *line, t_tools *tools, int *i)
{
	char	*str_to_be_expanded;
	char	*expanded_string;
	int		j;

	j = 0;
	str_to_be_expanded = malloc(sizeof(char) * 20);
	while ((is_whitespace(line[*i]) == FALSE && line[*i] != '\0')
		&& (line[*i] != '"' && line[*i] != '\''))
	{
		str_to_be_expanded[j] = line[*i];
		*i += 1;
		j++;
	}
	str_to_be_expanded[j] = '\0';
	expanded_string = expand_arg(str_to_be_expanded, tools);
	if (!expanded_string)
		exit(EXIT_FAILURE);
	// printf("Expanded: %s to %s\n", str_to_be_expanded, expanded_string);
	free(str_to_be_expanded);
	return (expanded_string);
}

int	inside_single_quote_only(char *string, char c)
{
	int		i;
	bool	single_quote;
	int		single_pos;
	bool	double_quote;
	int		double_pos;

	i = 0;
	single_quote = false;
	double_quote = false;
	while (string[i])
	{
		if (string[i] == '"')
		{
			double_quote = !double_quote;
			double_pos = i;
		}
		if (string[i] == '\'')
		{
			single_quote = !single_quote;
			single_pos = i;
		}
		if (string[i] == c)
		{
			if (single_quote == true && (double_quote == false
					|| (double_quote == true && single_pos < double_pos)))
				return (true);
			else
				return (false);
		}
		i++;
	}
	return (false);
}

char	*expand_heredoc(char *line, t_tools *tools)
{
	char	*expanded_string;
	char	*final_string;
	int		i;
	int		j;
	int		x;

	i = 0;
	j = 0;
	x = 0;
	final_string = malloc(sizeof(char) * ft_strlen(line) * 10);
	while (line[i] != '\0')
	{
		final_string[j] = line[i];
		if (line[i] == '$' && line[i + 1] != '\0'
			&& inside_single_quote_only(line, line[i]) == false)
		{
			expanded_string = get_expanded_arg(line, tools, &i);
			while (expanded_string && expanded_string[x])
			{
				final_string[j] = expanded_string[x];
				x++;
				j++;
			}
			x = 0;
		}
		else
		{
			i++;
			j++;
		}
	}
	final_string[j] = '\0';
	return (final_string);
}

int	hd_has_quotations(char *string)
{
	int	i;
	int	found_quotation;

	i = 0;
	found_quotation = FALSE;
	while (string[i])
	{
		if (find_token_type(string[i], string[i + 1]) == HEREDOC)
		{
			i += 2;
			while (is_whitespace(string[i]))
				i++;
			if (string[i] == '\'' || string[i] == '"')
				found_quotation = TRUE;
			else
				found_quotation = FALSE;
		}
		i++;
	}
	return (found_quotation);
}

int	create_heredoc(t_token *redirection, t_commands *cmd, t_tools *tools)
{
	int		file;
	char	*line;
	char	*path;
	pid_t	pid;

	file = 0;
	line = NULL;
	path = ft_calloc(15, sizeof(char));
	dprintf(2, "===>heredoc\tcmd=%s\tDelimiter=%s index=%d\n", cmd->cmds[0],
			redirection->cmd, redirection->index);
	if (!path)
		return (-1);
	pid = fork();
	if (pid == ERROR)
		return (ERROR);
	if (pid == 0)
	{
		path = ft_strjoin("/tmp/", ft_itoa(tools->heredoc));
		printf("path=%s\n", path);
		file = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (file < 0)
			return (error_file_handling(redirection->cmd));
		dprintf(2, "index=%d  tools index=%d\n", redirection->index,
				tools->heredoc);
		while (1)
		{
			line = readline("> ");
			if (ft_strncmp(line, redirection->cmd,
					ft_strlen(redirection->cmd)) == 0)
				break ;
			if (hd_has_quotations(tools->og_string) == TRUE)
			{
				// printf("Hd has quotations, literally copy paste.\n");
				write(file, line, ft_strlen(line));
				write(file, "\n", 1);
			}
			else
			{
				line = expand_heredoc(line, tools);
				printf("Line after expansion: %s\n", line);
				write(file, line, ft_strlen(line));
				write(file, "\n", 1);
			}
		}
		close(file);
		_exit(0);
	}
	redirection->index = tools->heredoc;
	tools->heredoc += 1; //check if status id ok
	wait(&pid);
	return (SUCCESS);
}
