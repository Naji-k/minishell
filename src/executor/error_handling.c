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

// int	is_directory(char *s_cmd)
// {
// 	struct stat	statbuf;

// 	if (stat(s_cmd, &statbuf) == 0)
// 	{
// 		if (S_ISDIR(statbuf.st_mode))
// 		{
// 			ft_putstr_fd(s_cmd, STDERR_FILENO);
// 			ft_putstr_fd(": is a directory\n", STDERR_FILENO);
// 			g_exit_status = 126;
// 			return (1);
// 		}
// 		else
// 		return (0);
// 	}
// 	return (S_ISDIR(statbuf.st_mode));
// }

int	is_directory(char *path)
{
	struct stat	statbuf;

	g_exit_status = 126;
	if (stat(path, &statbuf) != 0)
	{
		return (0);
	}
	if (S_ISDIR(statbuf.st_mode))
	{
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	e_cmd_not_found(char *s_cmd)
{
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	g_exit_status = 127;
	if (errno == EACCES)
	{
		if (is_directory(s_cmd))
			return (g_exit_status);
		else
			perror(s_cmd);
	}
	else if (errno == ENOENT)
	{
		ft_putstr_fd(s_cmd, STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else if (errno == 22)
	{
		ft_putstr_fd(s_cmd, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd(s_cmd, STDERR_FILENO);
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
