/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/04 21:01:02 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/04 21:01:02 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

void	bn_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("\nCurrent working dir: %s\n", cwd);
	else
		perror("getcwd() error");
}

int	bn_echo(t_commands **cmd_head)
{
	char **cmd;
	int i;

	i = 1;
	cmd = (*cmd_head)->cmds;
	while (cmd[i] != NULL)
	{
		if (ft_strncmp(cmd[1], "-n", 2) == 0)
			i++;
		ft_putstr_fd(cmd[i], 1);
		i++;
	}
	if (ft_strncmp(cmd[1], "-n", 2) != 0)
		ft_putstr_fd("\n", 1);

	return (0);
}