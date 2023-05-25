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

int	error_handling(char *str)
{
	ft_putstr_fd("Minishell: ", STDERR_FILENO);
	if (errno == 2)
	{
		perror(str);
		g_exit_status = 1;
	}
	if (errno == 9)
	{
		perror(str);
		g_exit_status = 1;
	}
	else {
		g_exit_status = errno;
		printf("exit_errno=%d\n", g_exit_status);
		perror(str);
	}
	return (ERROR);
}