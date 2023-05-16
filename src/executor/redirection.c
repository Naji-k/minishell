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
	while (redirection)
	{
		// if (node->redirection->type == REDIRECTION)
		{
			file = open(redirection->cmd, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (file < 0)
				ft_putstr_fd("file\n", 2);
			redirection = redirection->next;
		}
	}
	if (dup2(file, STDOUT_FILENO) == -1)
		ft_putstr_fd("redirection\n", 2);
	close(file);
}

void	ft_dup2_check(int old, int new)
{
	if (old != new)
	{
		if (dup2(old, new) == -1)
			ft_putstr_fd("dup2 check = - 1\n", 2);
	}
}
