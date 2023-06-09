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
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (true);
	while (str[i] != '\0')
	{
		if (ft_isdigit(str[i]))
			i++;
		else
			return (false);
	}
	return (true);
}

void	free_all_exit(t_tools *tools)
{
	free_env_list(&tools->env_list);
	if (tools->pwd != NULL)
		free(tools->pwd);
	if (tools->old_pwd != NULL)
		free(tools->old_pwd);
	free(tools);
	exit(g_exit_status);
}

void	error_exit(char *cmd)
{
	ft_putstr_fd("Minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
}

void	exit_with_number(t_tools *tools, char **simple_cmd)
{
	if (!tools->has_pipe)
		ft_putendl_fd("exit", STDERR_FILENO);
	if (simple_cmd[2] == NULL)
	{
		g_exit_status = ft_atoi(simple_cmd[1]);
		free_all_exit(tools);
	}
	else if (simple_cmd[2] != NULL)
	{
		g_exit_status = 1;
		ft_putstr_fd("Minishell: exit: too many arguments\n", STDERR_FILENO);
	}
}

int	mini_exit(t_tools *tools, char **simple_cmd)
{
	if (simple_cmd[1] == NULL)
	{
		if (!tools->has_pipe)
			ft_putendl_fd("exit", STDERR_FILENO);
		free_all_exit(tools);
	}
	else
	{
		if (is_all_numric(simple_cmd[1]) == true)
			exit_with_number(tools, simple_cmd);
		else if (is_all_numric(simple_cmd[1]) == false)
		{
			g_exit_status = 255;
			if (tools->has_pipe == false)
				ft_putendl_fd("exit", STDERR_FILENO);
			ft_putstr_fd("Minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(simple_cmd[1], STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			free_all_exit(tools);
		}
	}
	return (g_exit_status);
}
