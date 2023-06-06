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

int	is_heredoc(t_commands *cmd)
{

	if (cmd->redirections && cmd->redirections->type == HEREDOC)
	{
		// heredoc = cmd->redirections;
		fprintf(stderr, "HERE\n");
		while (cmd->redirections && cmd->redirections->type == HEREDOC)
		{
			if (create_heredoc(cmd->redirections, cmd) == -1)
				return (ERROR);
			cmd->redirections = cmd->redirections->next;
		}
		return (SUCCESS);
	}
	return (SUCCESS);
}

int	create_heredoc(t_token *redirection, t_commands *cmd)
{
	int		file;
	char	*line;
	pid_t	pid;

	file = 0;
	line = NULL;
	dprintf(2, "===>heredoc\tcmd=%s\tDelimiter=%s\n", cmd->cmds[0],
			redirection->cmd);
	pid = fork();
	if (pid == ERROR)
		return (ERROR);
	if (pid == 0)
	{
		file = open(redirection->cmd, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (file < 0)
			return (error_file_handling(redirection->cmd));
		while (1)
		{
			line = readline("> ");
			if (ft_strncmp(line, redirection->cmd,
					ft_strlen(redirection->cmd)) == 0)
				break ;
			write(file, line, ft_strlen(line));
			write(file, "\n", 1);
		}
		close(file);
	}
	wait(&pid);
	return (SUCCESS);
}
