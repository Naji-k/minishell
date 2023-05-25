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

void	redirection(t_commands *cmd)
{
	int		file;
	t_token	*redirection;

	redirection = cmd->redirections;
	printf("redirection_NAME: %s\n", redirection->cmd);
	printf("redirection_TYPE: %u\n", redirection->type);
	while (redirection)
	{
		if (redirection->type == REDIRECTION)
		{
			printf("\nRedirection\n");
			file = open(redirection->cmd, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (file < 0)
				ft_putstr_fd("file\n", 2);
			if (dup2(file, STDOUT_FILENO) == -1)
				ft_putstr_fd("error_redirection\n", 2);
			redirection = redirection->next;
		}
		else if (redirection->type == IN_FILE)
		{
			// printf("\ninfile\n");
			file = open(redirection->cmd, O_RDONLY, 0644);
			if (file < 0)
			{
				g_exit_status = errno;
				perror(redirection->cmd);
				printf("exit_error=%d",g_exit_status);
			}
			if (dup2(file, STDIN_FILENO) == -1)
				ft_putstr_fd("error_redirection\n", 2);
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
	int		n;
	char	message[10000];
	int		len_limiter;

	n = 1;
	len_limiter = ft_strlen(redirection->cmd);
	file = open("heredoc", O_CREAT | O_RDWR, 0777);
	while (ft_strncmp(message, redirection->cmd, len_limiter) != 0)
	{
		n = read(1, &message, 10000);
		if (ft_strncmp(message, redirection->cmd, len_limiter) != 0)
			write(file, &message, n);
	}
	return (file);
}
