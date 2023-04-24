/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 07:54:11 by ysrondy           #+#    #+#             */
/*   Updated: 2023/04/20 07:54:12 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Function that loops through all paths in 'PATH=' env variable
	and runs executable if it exists inside one of the paths.
	Currently only supports 1 command.
	I need to figure out how to use forks and pipes to have multiple commands work.
	Also need to turn this into a loop instead of exiting.
	Resources -> https://www.youtube.com/watch?v=iq7puCxsgHQ
				-> https://www.youtube.com/watch?v=Mqb2dVRe0uo
*/
void	execute(t_tools *tools, t_commands **cmd_head)
{
	char		*exec_string;
	int			i;

	i = 0;
	while (tools->paths[i] != NULL)
	{
		exec_string = ft_strjoin(tools->paths[i], (*cmd_head)->cmds[0]);
		if (access(exec_string, X_OK) == 0)
		{
			printf("---------------EXECUTION--------------------\n");
			if (execve(exec_string, (*cmd_head)->cmds, NULL) == -1)
			{
				perror("Error ->");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			i++;
			free(exec_string);
		}
	}
}
