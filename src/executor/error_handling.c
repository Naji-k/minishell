/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_handling.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/25 19:30:57 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/25 19:30:57 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	error_file_handling(char *str)
{
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	g_exit_status = 1;
	if (errno == ENOENT)
		perror(str);
	else if (errno == EBADF)
		perror(str);
	else if (errno == EACCES)
		perror(str);
	else if (errno == EISDIR)
		perror(str);
	else if (errno == EAGAIN)
		perror(str);
	else if (errno == ENOMEM)
		perror(str);
	else
	{
		g_exit_status = errno;
		printf("exit_errno=%d\n", g_exit_status);
		perror(str);
	}
	return (ERROR);
}

int	e_pipe_fork(char *str)
{
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	g_exit_status = 1;
	if (errno == EMFILE)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": Too many open files\n", STDERR_FILENO);
	}
	else if (errno == EAGAIN)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": Resource temporarily unavailable\n", STDERR_FILENO);
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": error forking\n", STDERR_FILENO);
	}
	return (g_exit_status);
}

int	e_heredoc(char *str)
{
	(void)str;
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	g_exit_status = 1;
	ft_putstr_fd(": error finding heredoc_path\n", STDERR_FILENO);
	return (ERROR);
}
