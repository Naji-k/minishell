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

int	should_expand(char *line, t_tools *tools, t_token *node, int i)
{
	if (((line[i] == '$' && line[i + 1] != '\0')
			|| (i == 0 && line[i] == '~'
				&& is_inside_quote(line, line[i + 1]) == NO_QUOTATION
				&& (line[i + 1] == '\0' || line[i + 1] == '/')))
		&& is_inside_quote(line, i) != SINGLE_QUOTE
		&& get_prev_node(tools->token_head, node)->type != HEREDOC)
		return (true);
	return (false);

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
