/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirection.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/03 18:29:22 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/03 18:29:22 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

int	redirection(t_commands *cmd)
{
	int		file;
	t_token	*redirection;

	redirection = cmd->redirections;
	// printf("redirection_NAME: %s\n", redirection->cmd);
	// printf("redirection_TYPE: %u\n", redirection->type);
	while (redirection)
	{
		if (redirection->type == REDIRECTION)
		{
			printf("\nRedirection\n");
			file = open(redirection->cmd, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (file < 0)
				return (error_handling(redirection->cmd));
			if (dup2(file, STDOUT_FILENO) == -1)
				ft_putstr_fd("error_redirection\n", 2);
			redirection = redirection->next;
		}
		else if (redirection->type == IN_FILE)
		{
			// printf("\ninfile\n");
			file = open(redirection->cmd, O_RDONLY, 0644);
			if (file < 0)
				(error_handling(redirection->cmd));
			if (dup2(file, STDIN_FILENO) == -1)
			{
				(error_handling(redirection->cmd));
			}
			redirection = redirection->next;
		}
		else if (redirection->type == HEREDOC)
		{
			// printf("\nheredoc\n");
			file = here_doc(redirection);
			printf("heredoc=%d\tdelimiter=%s\n", file, redirection->cmd);
			if (dup2(file, STDIN_FILENO) == -1)
				ft_putstr_fd("error_redirection\n", 2);
			redirection = redirection->next;
		}
		else if (redirection->type == A_REDIRECTION)
		{
			printf("\nA_Redirection\n");
			file = open(redirection->cmd, O_CREAT | O_RDWR | O_APPEND, 0644);
			if (file < 0)
				ft_putstr_fd("File_Appled\n", 2);
			if (dup2(file, STDOUT_FILENO) == -1)
				ft_putstr_fd("error_A_Redirection\n", 2);
			redirection = redirection->next;
		}
		else
		{
			printf("UNKNOW\n");
		}
	}
	close(file); // need to close previous files otherwise file leak.
	return (SUCCESS);
}

void	ft_dup2_check(int old, int new)
{
	if (old != new)
	{
		if (dup2(old, new) == -1)
			ft_putstr_fd("dup2 check = - 1\n", 2);
	}
}

int	here_doc(t_token *redirection)
{
	int		file;
	int		i;
	char	message[10000];

	i = 1;
	printf("heredoc\n");
	file = open("heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (file < 0)
	{
		g_exit_status = errno;
		perror(redirection->cmd);
	}
	dup2(file, STDOUT_FILENO);
	while (i)
	{
		read(STDIN_FILENO, message, 10000);
		if (ft_strncmp(message, redirection->cmd,
				ft_strlen(redirection->cmd)) == 0)
			i = 0;
	}
	return (0);
}
