/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   error_handling_extra.c                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/31 18:58:36 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/08/31 18:58:36 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

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

int	e_find_path(char *s_cmd)
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

int	e_cmd_not_found(char *s_cmd)
{
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	g_exit_status = 127;
	if (s_cmd[0] == '\0')
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	else if (errno == EACCES)
	{
		if (is_directory(s_cmd))
			return (g_exit_status);
		else
			perror(s_cmd);
	}
	else if (errno == ENOENT)
		perror(s_cmd);
	else if (errno == 22)
		perror(s_cmd);
	return (g_exit_status);
}
