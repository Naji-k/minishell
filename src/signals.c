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

void	handler_sigint(int s)
{
	// if (g_exit_status == 0)
	// 	printf("User clicked on Ctrl+c in heredoc.\n");
	(void)(s);
	printf("Pressed on ctrl+c\n");
	// rl_replace_line("Minishell:", 0);
	// rl_on_new_line();
	// rl_redisplay();
}

// Ctrl+D should only work if empty line.

void	handler_sigquit(int s)
{
	(void)s;
	printf("Pressed on ctrl+\'");
	exit(1);
}
