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
	else if (errno == EACCES)
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

int	is_directory(char *s_cmd)
{
	struct stat	statbuf;

	if (stat(s_cmd, &statbuf) == 0)
	{
		if (S_ISDIR(statbuf.st_mode))
		{
			ft_putstr_fd(s_cmd, STDERR_FILENO);
			ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		}
		else if (S_ISREG(statbuf.st_mode))
		{
			perror(s_cmd);
		}
		return (0);
	}
	return (S_ISDIR(statbuf.st_mode));
}

int	e_cmd_not_found(char *s_cmd)
{
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	if (errno == 2)
	{
		ft_putstr_fd(s_cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		g_exit_status = 127;
	}
	else if (errno == EACCES)
	{
		if (is_directory(s_cmd) != 0)
			perror(s_cmd);
		g_exit_status = 126;
	}
	else
	{
		ft_putstr_fd(s_cmd, STDERR_FILENO);
		ft_putstr_fd(ft_itoa(errno), STDERR_FILENO);
		g_exit_status = errno;
	}
	return (g_exit_status);
}

int	e_pipe_fork(char *str)
{
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	g_exit_status = 1;
	if (errno == EMFILE)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": Too many open files", STDERR_FILENO);
	}
	else if (errno == EAGAIN)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": Resource temporarily unavailable", STDERR_FILENO);
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putstr_fd(": Permission denied", STDERR_FILENO);
	}
	return (g_exit_status);
}
