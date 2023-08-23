/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_echo.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/09 18:49:22 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/09 18:49:22 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

/*
	check if there "-n" option for echo
	ex: echo -nnnnn hey (should not print with new_line)
 */
int	mini_echo_option(char *str)
{
	if (str[0] == '-' && str[1] == 'n')
	{
		str++;
		while (*str)
		{
			if (*str != 'n')
				return (0);
			str++;
		}
		return (1);
	}
	return (0);
}

/**
 * @brief just check if echo has any simple command
 * echo 
 * @param str 
 * @param tools 
 * @return int 
 */
int	mini_echo_checker(char *str, t_tools *tools)
{
	(void)tools;
	if (!str)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	return (1);
}

/**
 * @brief the main func for echo,
 * check if there any args for echo,,
 * call mini_echo_option(searching for -n)
 * @param tools 
 * @param simple_cmd ex: echo hey, echo -n hello
 * @return int 
 */
int	mini_echo(t_tools *tools, char **simple_cmd)
{
	int	i;

	i = 1;
	if (mini_echo_checker(simple_cmd[1], tools))
	{
		while (simple_cmd[i] && mini_echo_option(simple_cmd[i]) == 1)
			i++;
		while (simple_cmd[i] != NULL)
		{
			ft_putstr_fd(simple_cmd[i], STDOUT_FILENO);
			if (simple_cmd[i + 1] != NULL && simple_cmd[i][0] != '\0')
				ft_putstr_fd(" ", STDOUT_FILENO);
			i++;
		}
		if (mini_echo_option(simple_cmd[1]) == 0)
			ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	return (SUCCESS);
}
