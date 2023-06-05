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
			dprintf(2, "\nRedirection\n");
			file = open(redirection->cmd, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (file < 0)
				return (error_file_handling(redirection->cmd));
			ft_dup2_check(file, STDOUT_FILENO);
			redirection = redirection->next;
		}
		else if (redirection->type == IN_FILE)
		{
			dprintf(2, "\ninfile\n");
			file = open(redirection->cmd, O_RDONLY, 0644);
			if (file < 0)
				return (error_file_handling(redirection->cmd));
			ft_dup2_check(file, STDIN_FILENO);
			redirection = redirection->next;
		}
		else if (redirection->type == HEREDOC)
		{
			file = open(redirection->cmd, O_RDONLY, 0644);
			if (file < 0)
				return (error_file_handling(redirection->cmd));
			ft_dup2_check(file, STDIN_FILENO);
			close(file);
			redirection = redirection->next;
		}
		else if (redirection->type == A_REDIRECTION)
		{
			dprintf(2, "\nA_Redirection\n");
			file = open(redirection->cmd, O_CREAT | O_RDWR | O_APPEND, 0644);
			if (file < 0)
				return (error_file_handling(redirection->cmd));
			ft_dup2_check(file, STDOUT_FILENO);
			redirection = redirection->next;
		}
		else
			dprintf(2, "UNKNOW\n");
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
