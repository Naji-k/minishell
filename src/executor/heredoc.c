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
				if (create_heredoc(heredoc, tools) == -1)
					return (ERROR);
				heredoc = heredoc->next;
			}
		}
		s_cmd = s_cmd->next;
	}
	return (SUCCESS);
}

int	hd_has_quotations(char *string)
{
	int	i;
	int	found_quotation;

	i = 0;
	found_quotation = false;
	while (string[i])
	{
		if (find_token_type(string[i], string[i + 1]) == HEREDOC)
		{
			i += 2;
			while (is_whitespace(string[i]))
				i++;
			if (string[i] == '\'' || string[i] == '"')
				found_quotation = true;
			else
				found_quotation = false;
		}
		i++;
	}
	return (found_quotation);
}

void	start_heredoc_loop(t_tools *tools, t_token *redirection, int file)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (ft_strncmp(line, redirection->cmd,
				ft_strlen(line)) == 0)
			break ;
		if (hd_has_quotations(tools->og_string) == true)
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
}

int	execute_hd_process(t_tools *tools, t_token *redirection)
{
	int		file;
	char	*path;
	char	*path_hd;

	path_hd = ft_itoa(tools->heredoc);
	
	if (!path_hd)
		exit(EXIT_FAILURE); // need to change this one too
	path = ft_strjoin("/tmp/", path_hd);
	if (!path)
		exit(EXIT_FAILURE); // need to change
	file = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file < 0)
	{
		free(path_hd);
		free(path);
		return (error_file_handling(redirection->cmd));
	}
	start_heredoc_loop(tools, redirection, file);
	close(file);
	return (0);
}

int	create_heredoc(t_token *redirection, t_tools *tools)
{
	int		success;
	pid_t	pid;
	int		status;

	signal(SIGINT, &handler_hd_sigint);
	pid = fork();
	if (pid == ERROR)
		return (ERROR);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		success = execute_hd_process(tools, redirection);
		if (success == 0)
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
