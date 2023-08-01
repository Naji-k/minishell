/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/26 13:24:27 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/04/26 13:24:27 by nakanoun      ########   odam.nl         */
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
	this file going to be the main one for the executor, so from here,
	I would like to make some checks
	ex: 1 cmd, multi cmnds, direction?
	and from here call the specific functions
 */

/*
	executor: MAIN FUNC for executing part..
 */
void	executor(t_tools *tools, t_commands **cmd_head)
{
	int	fd_i;
	int	fd_o;

	if (!(*cmd_head))
		return ;
	if (is_heredoc(cmd_head, tools) == ERROR)
		return ;
	fd_i = dup(STDIN_FILENO);
	fd_o = dup(STDOUT_FILENO);
	if ((*cmd_head)->next == NULL)
		one_cmd_handler(tools, cmd_head);
	if ((*cmd_head)->next != NULL)
		multi_commands_handler(tools, cmd_head);
	dup2(fd_i, STDIN_FILENO);
	dup2(fd_o, STDOUT_FILENO);
}

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

void	execve_cmd(t_tools *tools, t_commands **cmd_head)
{
	char	*cmd_path;

	cmd_path = find_cmd_path(tools, (*cmd_head)->cmds[0]);
	if (!cmd_path)
		exit(e_cmd_not_found((*cmd_head)->cmds[0]));
	tools->envp = env_list_to_array(&tools->env_list);
	if (execve(cmd_path, (*cmd_head)->cmds, tools->envp) == -1)
		e_cmd_not_found((*cmd_head)->cmds[0]);
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
/*
	Here i am checking if the executor is a local(in project dir) | from env->PATH
 */

char	*check_current_dir(char *cmd)
{
	if (access(cmd, F_OK) == -1)
		return (NULL);
	return (cmd);
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

/*
	find the path of the command...(if it is not local executable)
 */
char	*find_cmd_path(t_tools *tools, char *cmd)
{
	char	*cmd_path;
	char	*tmp;
	int		i;

	// dprintf(2, "find_cmd[0]=%s\n", cmd);
	cmd_path = NULL;
	cmd_path = check_current_dir(cmd);
	if (cmd_path)
		return (cmd_path);
	tools->paths = get_paths2(&tools->env_list);
	if (tools->paths)
	{
		i = -1;
		while (tools->paths[++i])
		{
			tmp = ft_strjoin(tools->paths[i], cmd);
			cmd_path = tmp;
			if (access(cmd_path, F_OK) == 0)
			{
				free_2d_arr(tools->paths);
				return (cmd_path);
			}
			free(tmp);
		}
	}
	free_2d_arr(tools->paths);
	return (NULL);
}

int	close_pipes(int *fd, int old_fd)
{
	if (fd)
	{
		if (close(fd[0] == ERROR))
			ft_putstr_fd("close fails\n", STDERR_FILENO);
		if (close(fd[1] == ERROR))
			ft_putstr_fd("close fails\n", STDERR_FILENO);
	}
	if (close(old_fd) == ERROR)
		ft_putstr_fd("close fails\n", STDERR_FILENO);
	return (0);
}
