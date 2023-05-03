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
	int	file;

	if (cmd->redirections)
	{
		// if (node->redirection->type == REDIRECTION)
		{
			file = open(cmd->redirections->cmd, O_CREAT | O_WRONLY | O_TRUNC,
					0644);
			if (file < 0)
				ft_putstr_fd("file\n", 2);
			if (dup2(file, STDOUT_FILENO) == -1)
				ft_putstr_fd("dup2_file\n", 2);
			close(file);
		}
	}
}

void	ft_dup2_check(int old, int new)
{
	if (old != new)
	{
		if (dup2(old, new) == -1)
			ft_putstr_fd("dup2 - 1\n", 2);
	}
}

void	ft_pwd(void)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("\nCurrent working dir: %s\n", cwd);
	else
		perror("getcwd() error");
}