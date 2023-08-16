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
#include "minishell.h"

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

char	*get_expanded_arg(char *line, t_tools *tools, int *i, t_token *node)
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
		if (line[*i] == '~')
		{
			j++;
			break ;
		}
		(*i) += 1;
		j++;
		if (line[*i] == '$' || line[*i] == '-' || line[*i] == '~')
		{
			if (line[*i] == '$')
			{
				str_to_be_expanded[j] = line[*i];
				(*i) += 1;
				j++;
			}
			break ;
		}
	}
	str_to_be_expanded[j] = '\0';
	printf("String to be expanded: %s\n", str_to_be_expanded);
	expanded_string = expand_arg(str_to_be_expanded, tools);
	if (!expanded_string)
	{
		node->valid = false;
		if (get_prev_node(tools->token_head, node)->type != LITERAL && get_prev_node(tools->token_head, node)->type != PIPE)
			return (str_to_be_expanded);
		str_to_be_expanded[0] = '\0';
		return (str_to_be_expanded);
	}
	printf("Expanded: %s to %s\n", str_to_be_expanded, expanded_string);
	free(str_to_be_expanded);
	return (expanded_string);
}

int	is_inside_quote(char *string, int pos_char)
{
	int		i;
	bool	single_quote;
	bool	double_quote;

	i = 0;
	single_quote = false;
	double_quote = false;
	// printf("Line: %s\n", string);
	while (string[i])
	{
		// printf("String[i]: %d || Pos_char: %d\n", i, pos_char);
		if (string[i] == '"')
		{
			if (single_quote == true)
				;
			else
			{
				// printf("Found Double Quote!\n");
				double_quote = !double_quote;
			}
		}
		if (string[i] == '\'')
		{
			if (double_quote == true)
				;
			else
			{
				// printf("Found a single quote!\n");
				single_quote = !single_quote;
			}
		}
		if (i == pos_char)
		{
			// printf("Single Quote: %d, Double Quote: %d, Single Pos: %d, Double Pos: %d\n", single_quote, double_quote, single_pos, double_pos);
			if (single_quote == true)
			{
				// printf("We are inside single quote.\n");
				return (SINGLE_QUOTE);
			}
			else if (double_quote == true)
			{
				// printf("We are inside double quote.\n");
				return (DOUBLE_QUOTE);
			}
			else
			{
				// printf("There are no quotes.\n");
				return (NO_QUOTATION);
			}
		}
		i++;
	}
	return (false);
}

char	*ft_str_add_char(char *str, char c)
{
	char	*new_string;
	int		i;

	new_string = malloc(sizeof(char) * (ft_strlen(str) + 2));
	i = 0;
	while (str[i])
	{
		new_string[i] = str[i];
		i++;
	}
	new_string[i] = c;
	new_string[i + 1] = '\0';
	free(str);
	return (new_string);
}

char	*expand_heredoc(t_token *node, char *line, t_tools *tools)
{
	char	*expanded_string;
	char	*final_string;
	int		i;

	i = 0;
	final_string = calloc((ft_strlen(line) + 1), sizeof(char));
	expanded_string = NULL;
	while (line[i] != '\0')
	{
		// printf("Top Line[i]: %c || %d\n", line[i], i);
		if (((line[i] == '$' && line[i + 1] != '\0')
				|| (i == 0 && line[i] == '~'
					&& is_inside_quote(line, line[i + 1]) == NO_QUOTATION
					&& (line[i + 1] == '\0' || line[i + 1] == '/')))
			&& is_inside_quote(line, i) != SINGLE_QUOTE
			&& get_prev_node(tools->token_head, node)->type != HEREDOC)
		{
			// printf("Going to expand.\n");
			expanded_string = get_expanded_arg(line, tools, &i, node);
			final_string = ftp_strjoin(final_string, expanded_string);
			free(expanded_string);
			if (line[i] == '~' && i == 0)
				i++;
			// printf("MID Line[i]: %c || %d\n", line[i], i);
		}
		else
		{
			// printf("Normal Line[i]: %c || %d\n", line[i], i);
			final_string = ft_str_add_char(final_string, line[i]);
			i++;
		}
	}
	free(line);
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
	char	*path_hd;
	pid_t	pid;
	int		status;

	file = 0;
	line = NULL;
	path_hd = NULL;
	path = NULL;
	dprintf(2, "===>heredoc\tcmd=%s\tDelimiter=%s index=%d\n", cmd->cmds[0],
		redirection->cmd, redirection->index);
	if (!path)
		return (-1);
	signal(SIGINT, &handler_hd_sigint);
	pid = fork();
	if (pid == ERROR)
		return (ERROR);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		path_hd = ft_itoa(tools->heredoc);
		path = ft_strjoin("/tmp/", path_hd);
		printf("path=%s\n", path);
		file = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (file < 0)
		{
			free(path_hd);
			free(path);
			return (error_file_handling(redirection->cmd));
		}
		dprintf(2, "index=%d  tools index=%d\n", redirection->index,
				tools->heredoc);
		while (1)
		{
			line = readline("> ");
			if (ft_strncmp(line, redirection->cmd, ft_strlen(redirection->cmd)) == 0)
				break ;
			if (hd_has_quotations(tools->og_string) == TRUE)
			{
				write(file, line, ft_strlen(line));
				write(file, "\n", 1);
			}
			else
			{
				line = expand_heredoc(*(tools->token_head), line, tools);
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
	waitpid(pid, &status, 0);
	signal(SIGINT, &handler_sigint);
	if (WIFSIGNALED(status))
		return (ERROR);
	return (SUCCESS);
}
