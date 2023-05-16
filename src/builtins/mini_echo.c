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
	while (*str != '\0')
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
	int		i;
	char	*tmp;

	i = 1;
	tmp = "";
	(void)tools;
	if (simple_cmd[1] == '\0')
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return (0);
	}
	while (mini_echo_option(simple_cmd[i]) == 0)
		i++;
	while (simple_cmd[i] != NULL && simple_cmd[i] != '\0')
	{
		if (simple_cmd[i][0] == '~')
		{
			tmp = getenv("HOME");
			ft_putstr_fd(tmp, STDOUT_FILENO);
			simple_cmd[i] = ft_substr(simple_cmd[i], 1,
					ft_strlen(simple_cmd[i]));
		}
		ft_putstr_fd(simple_cmd[i], STDOUT_FILENO);
		if (simple_cmd[i + 1] != NULL)
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (mini_echo_option(simple_cmd[1]) != 0)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
