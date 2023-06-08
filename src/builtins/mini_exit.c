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

static bool	is_all_numric(char *str)
{
	while (*str)
	{
		if (ft_isdigit(*str))
			str++;
		else
			return (false);
	}
	return (true);
}
/* static int	exit_args(char **simple_cmd)
{
	int	i;

	i = 1;
	while (simple_cmd[i] != NULL)
	{
		if (is_all_numric(simple_cmd[i]))
			i++;
	}
	return (0);
}
 */
int	mini_exit(t_tools *tools, char **simple_cmd)
{
	ft_putendl_fd("exit", STDERR_FILENO);
	if (simple_cmd[1] == NULL)
	{
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
	else
	{
		// exit_args(simple_cmd);
		if (is_all_numric(simple_cmd[1]) && simple_cmd[2] == '\0')
		{
			g_exit_status = ft_atoi(simple_cmd[1]);
			exit(g_exit_status);
		}
		else if (is_all_numric(simple_cmd[1]) && simple_cmd[2] != '\0')
		{
			g_exit_status = 1;
			ft_putstr_fd("Minishell: exit: too many arguments", STDERR_FILENO);
		}
		if (!is_all_numric(simple_cmd[1]) && !is_all_numric(simple_cmd[2]))
		{
			g_exit_status = 255;

			// bash: exit: asd: numeric argument required
			ft_putstr_fd("Minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(simple_cmd[1], STDERR_FILENO);
			ft_putstr_fd(" numeric argument required", STDERR_FILENO);
		}
	}
	return (0);
}
/* 
1)	if 1st args ONLY NUMBERS and less than 255 => exit_coda = cmd
2)	if more args contain any digit => exit_code = 255
3) if
exit anyway but
if args numric false then print 
exit 40: (exit_code)
exit asd 
exit | exit
exit 34 asd 
exit (MAX 2 args 1st alpha, 2nd num)
exit: can exit if there one NUM ARGS but should check for NON-NUMERIC ARGS
 */