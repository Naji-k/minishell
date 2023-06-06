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

int	is_heredoc(t_commands **cmd)
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
				if (create_heredoc(heredoc, s_cmd) == -1)
					return (ERROR);
				heredoc = heredoc->next;
			}
		}
		s_cmd = s_cmd->next;
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
			/* TODO:
			I am Creating a file named as HEREDOC_DELIMITER, 
			but inside the file you have to expand the inputs,,
			line= should be expanded from env, also should take care of ' "" 
			ex:
			cat << 'EOF'
			cat << EOF
			IN OUR MINISHELL YOU CAN SEE THE INPUTS IN A FILE SO DONT HAVE TO USE cat 
			<< EOF , << 'EOF'
			 */
			write(file, line, ft_strlen(line));
			write(file, "\n", 1);
		}
		close(file);
	}
	wait(&pid);
	return (SUCCESS);
}
