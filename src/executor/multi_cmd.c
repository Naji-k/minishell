/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   multi_cmd.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/03 16:26:54 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/08/03 16:26:54 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"
#include <fcntl.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
	this func will split the commands to two parts:
	LOOP: on all commands -1 ex:lst command
	and run the last command separately
 */

void	multi_commands_handler(t_tools *tools, t_commands **cmd_head)
{
	t_commands	*node;
	int			old_fd;
	int			fd[2];
	pid_t		last_pid;

	old_fd = STDIN_FILENO;
	node = *cmd_head;
	dprintf(2, "MULTI\n");
	tools->has_pipe = true;
	while (node->next != NULL)
	{
		if (pipe(fd) == ERROR)
			error_file_handling("pipe");
		if (multi_pipex_process(tools, &node, old_fd, fd) == ERROR)
		{
			// CLOSE PIPES
			return ;
		}
		close(fd[1]);
		if (node != *cmd_head)
			close(old_fd);
		old_fd = fd[0];
		node = node->next;
	}
	last_pid = last_cmd(tools, &node, old_fd);
	wait_last_pid(last_pid);
}

int	multi_pipex_process(t_tools *tools, t_commands **cmd_head, int old_fd,
		int *fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == ERROR)
	{
		close_pipes(fd, old_fd);
		return (error_file_handling("fork"));
	}
	if (pid == 0)
	{
		close(fd[0]);
		ft_dup2_check(old_fd, STDIN_FILENO);
		ft_dup2_check(fd[1], STDOUT_FILENO);
		if ((*cmd_head)->redirections)
			if (redirection((*cmd_head)))
				exit(EXIT_FAILURE);
		if ((*cmd_head)->builtin)
			exit(run_builtin((*cmd_head)->cmds[0], tools, (*cmd_head)->cmds));
		if ((*cmd_head)->cmds[0])
			execve_cmd(tools, cmd_head);
		else
			exit(EXIT_FAILURE);
	}
	return (SUCCESS);
}
/*
	After finish the loop on all (commands-1) so now just execute the last command
	and returns the last PID to wait for it
 */

pid_t	last_cmd(t_tools *tools, t_commands **last_cmd, int old_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == ERROR)
		error_file_handling("fork");
	if (pid == 0)
	{
		ft_dup2_check(old_fd, STDIN_FILENO);
		if ((*last_cmd)->redirections)
			if (redirection((*last_cmd)))
				exit(EXIT_FAILURE);
		if ((*last_cmd)->builtin)
			exit(run_builtin((*last_cmd)->cmds[0], tools, (*last_cmd)->cmds));
		execve_cmd(tools, last_cmd);
	}
	close(old_fd);
	return (pid);
}

void	wait_last_pid(pid_t last_pid)
{
	int	status;

	waitpid(last_pid, &status, 0);
	while (wait(NULL) > 0)
		;
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}
