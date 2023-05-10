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

#include "buildin.h"
#include "executor.h"

/* 
	check if there "-n" option for echo
	ex: echo -nnnnn hey (should not print with new_line)
 */
int	mini_echo_option(char *str)
{
	while (*str)
	{
		if (str[0] == '-' && str[1] == 'n')
			str++;
		if (*str != 'n')
			return (1);
		str++;
	}
	return (0);
}

int	mini_echo(t_tools *tools, char **simple_cmd)
{
	int	i;

	i = 1;
	(void)tools;
	if (mini_echo_option(simple_cmd[1]) == 0)
		i++;
	while (simple_cmd[i] != NULL)
	{
		ft_putstr_fd(simple_cmd[i], 1);
		if (simple_cmd[i + 1] != NULL)
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (mini_echo_option(simple_cmd[1]) != 0)
		ft_putstr_fd("\n", 1);
	return (0);
}
