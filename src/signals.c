/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/02 20:17:05 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/08/02 20:17:06 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"

void	handler_sigint(int s)
{
	(void)(s);
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	g_exit_status = 1;
}

// Ctrl+D should only work if empty line.

void	handler_hd_sigint(int s)
{
	(void)(s);
}


void	handler_sigquit(int s)
{
	(void)s;
	ft_putendl_fd("exit", STDERR_FILENO);
	// rl_on_new_line();
	exit(0);
}
