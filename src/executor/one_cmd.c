/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   one_cmd.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/03 16:47:52 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/08/03 16:47:52 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include <fcntl.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>

void	one_cmd_handler(t_tools *tools, t_commands **cmd_head)
{
	tools->has_pipe = false;
	if ((*cmd_head)->redirections)
		if (redirection((*cmd_head)))
			return ;
	if ((*cmd_head)->builtin)
	{
		g_exit_status = run_builtin((*cmd_head)->cmds[0], tools,
				(*cmd_head)->cmds);
		return ;
	}
	if ((*cmd_head)->cmds[0][0] == '\0')
	{
		e_cmd_not_found((*cmd_head)->cmds[0]);
		return ;
	}
	if ((*cmd_head)->cmds[0])
		execute_onc_cmd(tools, cmd_head);
}

/*
	If it is only one cmnd, you don't have to fork...
	so just execute on the main process
 */
void	execute_onc_cmd(t_tools *tools, t_commands **cmd_head)
{
	char		*cmd_path;
	t_commands	*node;
	pid_t		pid;
	int			status;

	node = *cmd_head;
	cmd_path = find_cmd_path(tools, node->cmds[0]);
	if (!cmd_path)
	{
		e_cmd_not_found(node->cmds[0]);
		return ;
	}
	pid = fork();
	if (pid == ERROR)
		error_file_handling("fork");
	if (pid == 0)
	{
		tools->envp = env_list_to_array(&tools->env_list);
		if (execve(cmd_path, node->cmds, tools->envp) == -1)
			exit(e_cmd_not_found(node->cmds[0]));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}
