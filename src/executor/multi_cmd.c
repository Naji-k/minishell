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

/**
 * @brief to handle multi commands by splitting them into two parts:
 * has_pipe=true;
	- loop: on all (commands -1), and last command
	- run the last command separately

 * old_fd: Is a reference, between two pipes
 * (save the value of STD_OUT fd from the old_pipe,
 *  and dup it as STD_IN for new_pipe) 
 * @param tools 
 * @param cmd_head includes: **cmd, *redirection,  *builtin
 */

void	multi_commands_handler(t_tools *tools, t_commands **cmd_head)
{
	t_commands	*node;
	int			old_fd;
	int			fd[2];
	pid_t		last_pid;

	old_fd = STDIN_FILENO;
	node = *cmd_head;
	tools->has_pipe = true;
	while (node->next != NULL)
	{
		if (pipe(fd) == ERROR)
			error_file_handling("pipe");
		if (multi_pipes_process(tools, &node, old_fd, fd) == ERROR)
			return ;
		close(fd[1]);
		if (node != *cmd_head)
			close(old_fd);
		old_fd = fd[0];
		node = node->next;
	}
	last_pid = last_cmd(tools, &node, old_fd);
	wait_last_pid(last_pid);
}

/**
 * This function is the main executor for multi- commands,
 * STD_IN is from old_fd, and 
 * 
 * @param tools 
 * @param cmd_head Includes: **cmd, *redirection, *builtin
 * @param old_fd reference for STD_IN, and it will
 * @param fd executor fd
 * @return if forks fails=> ERROR, SUCCESS,
 */
int	multi_pipes_process(t_tools *tools, t_commands **cmd_head, int old_fd,
		int *fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == ERROR)
	{
		close_pipes(fd, old_fd);
		return (e_pipe_fork("fork"), ERROR);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		close(fd[0]);
		if (ft_dup2_check(old_fd, STDIN_FILENO) == ERROR || ft_dup2_check(fd[1],
				STDOUT_FILENO) == ERROR)
			exit(EXIT_FAILURE);
		if ((*cmd_head)->redirections)
			if (redirection((*cmd_head)))
				exit(EXIT_FAILURE);
		if ((*cmd_head)->builtin)
			exit(run_builtin((*cmd_head)->cmds[0], tools, (*cmd_head)->cmds));
		if ((*cmd_head)->cmds[0])
			execve_m_cmds(tools, cmd_head);
		else
			exit(EXIT_FAILURE);
	}
	return (SUCCESS);
}

void	execve_m_cmds(t_tools *tools, t_commands **cmd_head)
{
	char	*cmd_path;

	cmd_path = find_cmd_path(tools, (*cmd_head)->cmds[0]);
	if (!cmd_path)
		exit(e_find_path((*cmd_head)->cmds[0]));
	tools->envp = env_list_to_array(tools->env_list);
	if (execve(cmd_path, (*cmd_head)->cmds, tools->envp) == -1)
		exit(e_cmd_not_found((*cmd_head)->cmds[0]));
}

/**
 * @brief here is the 2nd phase of multi_command_process
 * after finish the loop on all (command -1), 
 * so now just execute the last command,
 * @param tools 
 * @param last_cmd 
 * @param old_fd reference to STD_OUT for (last_command -1),
 * 		 it will dup2 as STD_IN
 * @return pid_t returns the last PID to wait for it
 */

pid_t	last_cmd(t_tools *tools, t_commands **last_cmd, int old_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == ERROR)
		error_file_handling("fork");
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		ft_dup2_check(old_fd, STDIN_FILENO);
		if ((*last_cmd)->redirections)
			if (redirection((*last_cmd)))
				exit(EXIT_FAILURE);
		if ((*last_cmd)->builtin)
			exit(run_builtin((*last_cmd)->cmds[0], tools, (*last_cmd)->cmds));
		execve_m_cmds(tools, last_cmd);
	}
	close(old_fd);
	return (pid);
}

/**
 * @brief wait_last_pid and update g_exit_status,
 * 
 * @param last_pid of the last_command
 */
void	wait_last_pid(pid_t last_pid)
{
	int	status;

	waitpid(last_pid, &status, 0);
	while (wait(NULL) > 0)
		;
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	signal_checker(status);
	signal(SIGINT, handler_sigint);
}
