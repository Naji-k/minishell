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

	if ((*cmd_head) == NULL)
		exit(0);
	if (is_heredoc((*cmd_head)) == ERROR)
		return ;
	fd_i = dup(STDIN_FILENO);
	fd_o = dup(STDOUT_FILENO);
	tools->envp = env_list_to_array(&tools->env_list);
	if ((*cmd_head)->next == NULL)
	{
		if ((*cmd_head)->redirections)
			if (redirection((*cmd_head)))
				return ;
		if ((*cmd_head)->builtin)
			g_exit_status = execute_builtin((*cmd_head)->cmds[0])(tools,
																	(*cmd_head)->cmds);
		if ((*cmd_head)->cmds[0])
			execute_onc_cmd(tools, cmd_head);
	}
	if ((*cmd_head)->next != NULL)
		multi_comands(tools, cmd_head);
	dup2(fd_i, STDIN_FILENO);
	dup2(fd_o, STDOUT_FILENO);
	// free_2d_arr(tools->envp);
}
/* 
	this func will split the commands to two parts: 
	LOOP: on all commands -1 ex:lst command
	and run the last command separately
 */

void	multi_comands(t_tools *tools, t_commands **cmd_head)
{
	t_commands	*node;
	int			old_fd;
	int			fd[2];
	pid_t		last_pid;

	old_fd = STDIN_FILENO;
	node = *cmd_head;
	dprintf(2, "MULTI\n");
	while (node->next != NULL)
	{
		if (pipe(fd) == -1)
			ft_putstr_fd("pipe:\n", 2);
		multi_pipex_process(tools, &node, old_fd, fd);
		close(fd[1]);
		if (node != *cmd_head)
		{
		if (is_heredoc(node) == ERROR)
			return ;
		close(old_fd);
		}
		old_fd = fd[0];
		node = node->next;
	}
	if (is_heredoc(node) == ERROR)
		return ;
	if (node->redirections)
		if (redirection(node))
			return ;
	last_pid = last_cmd(tools, &node, old_fd);
	wait_last_pid(last_pid);
}

void	multi_pipex_process(t_tools *tools, t_commands **cmd_head, int old_fd,
		int *fd)
{
	char	*cmd_path;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		ft_putstr_fd("fork\n", 2);
	if (pid == 0)
	{
		close(fd[0]);
		ft_dup2_check(old_fd, STDIN_FILENO);
		ft_dup2_check(fd[1], STDOUT_FILENO);
		if ((*cmd_head)->redirections)
			if (redirection((*cmd_head)))
				exit(EXIT_FAILURE);
		if ((*cmd_head)->builtin)
		{
			//should return the return value from builtin
			exit(execute_builtin((*cmd_head)->cmds[0])(tools,
														(*cmd_head)->cmds));
		}
		if ((*cmd_head)->cmds[0])
		{
			cmd_path = find_cmd_path(tools, (*cmd_head)->cmds[0]);
			if (!cmd_path)
				exit(e_cmd_not_found((*cmd_head)->cmds[0]));
			if (execve(cmd_path, (*cmd_head)->cmds, tools->envp) == -1)
				e_cmd_not_found((*cmd_head)->cmds[0]);
		}
		else
			exit(1);
	}
}
/* 
	After finish the loop on all (commands-1) so now just execute the last command
 */

pid_t	last_cmd(t_tools *tools, t_commands **last_cmd, int old_fd)
{
	char	*cmd_path;
	pid_t	pid;
	int		i;

	pid = fork();
	if (pid == 0)
	{
		ft_dup2_check(old_fd, STDIN_FILENO);
		if ((*last_cmd)->builtin)
		{
			i = execute_builtin((*last_cmd)->cmds[0])(tools, (*last_cmd)->cmds);
			exit(i);
		}
		dprintf(2, "last_cmd[0]=%s last_cmd[1]=%s\n", (*last_cmd)->cmds[0],
				(*last_cmd)->cmds[1]);
		cmd_path = find_cmd_path(tools, (*last_cmd)->cmds[0]);
		if (!cmd_path)
			e_cmd_not_found((*last_cmd)->cmds[0]);
		if (cmd_path)
		{
			if (execve(cmd_path, (*last_cmd)->cmds, tools->envp) == -1)
				e_cmd_not_found((*last_cmd)->cmds[0]);
		}
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
	if (cmd_path)
	{
		pid = fork();
		if (pid == 0)
		{
			if (execve(cmd_path, node->cmds, tools->envp) == -1)
				exit(e_cmd_not_found(node->cmds[0]));
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
	}
}

/*
	find the path of the command...(if it is not local executable)
 */
char	*find_cmd_path(t_tools *tools, char *cmd)
{
	char	*cmd_path;
	int		i;

	// dprintf(2, "find_cmd[0]=%s\n", cmd);
	tools->paths = find_path(tools->envp);
	cmd_path = check_current_dir(cmd);
	if (cmd_path)
		return (cmd_path);
	if (tools->paths)
	{
		i = -1;
		while (tools->paths[++i])
		{
			cmd_path = ft_strjoin(tools->paths[i], cmd);
			free(tools->paths[i]);
			if (access(cmd_path, F_OK) == 0)
				return (cmd_path);
			free(cmd_path);
		}
	}
	//should free
	return (NULL);
}
