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
	if (errno == 2)
	{
		perror(str);
		g_exit_status = 1;
	}
	else if (errno == 9)
	{
		perror(str);
		g_exit_status = 1;
	}
	else if (errno == 13)
	{
		perror(str);
		g_exit_status = 1;
	}
	else
	{
		g_exit_status = errno;
		printf("exit_errno=%d\n", g_exit_status);
		perror(str);
	}
	return (ERROR);
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
	else if (errno == 13)
	{
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


