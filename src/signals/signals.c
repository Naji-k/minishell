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

void	handler_hd_sigint(int s)
{
	(void)(s);
	printf("\n");
}

void	handler_sigquit(int s)
{
	(void)s;
	ft_putendl_fd("exit", STDERR_FILENO);
	exit(0);
}

/*
fix export abc (then print env)
Fix export +=, a in env list, a= in both env list and export
Fix ./minishell ./minishell ./minishell

*/
