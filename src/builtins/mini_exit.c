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
	// free_token_list(&tools->lexer_list);
	// free_token_list(&cmds_head->redirections);
	// free_cmd_list(&cmds_head);
	// free_2d_arr(tools->envp);
	// free_2d_arr(tools->paths);
	// free_env_list(&tools->env_list);
	exit(EXIT_SUCCESS);
	return (0);
}
