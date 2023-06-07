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
	(void)simple_cmd;
	printf("STATUS=%d\n", g_exit_status);
	ft_putendl_fd("exit", 1);
	// tools->loop = false;
	// fprintf(stderr, "%p\n", tools->envp[0]);
	// free_2d_arr(tools->envp); //keep this
	// free_2d_arr(tools->paths);	//keep this
	free_env_list(&tools->env_list);
	free(tools->pwd);
	free(tools->old_pwd);
	free(tools);
	exit(0);
}
/* 
exit 40: (exit_code)
exit asd : do not exit
exit | exit
exit 34 asd 
exit (MAX 2 args 1st alpha, 2nd num)
exit: can exit if there one NUM ARGS but should check for NON-NUMERIC ARGS
 */