/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_exit.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/18 21:10:14 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/18 21:10:14 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

int	mini_exit(t_tools *tools, char **simple_cmd)
{
	(void)tools;
	(void)simple_cmd;
	ft_putendl_fd("exit", STDERR_FILENO);
	tools->loop = false;
	return (0);
}
