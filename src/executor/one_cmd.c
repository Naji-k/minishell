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

/**
 * @brief This function handles one command: has pipe=false,
 * if there is redirection, or it is built-ins, or empty.
 * 
 * @param tools 
 * @param cmd_head includes: **cmd, *redirection,  *builtin
 */
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
		return ;
	if ((*cmd_head)->cmds[0])
		execute_onc_cmd(tools, cmd_head);
}

/**
 * @brief Executor for one command, find a path, convert env to char **env,
	and execve
 * 
 * @param tools 
 * @param cmd_head includes: **cmd, *redirection,  *builtin
 */
void	execute_onc_cmd(t_tools *tools, t_commands **cmd_head)
{
	char		*cmd_path;
	t_commands	*one_cmd;
	pid_t		pid;
	int			status;

	one_cmd = *cmd_head;
	pid = fork();
	if (pid == ERROR)
		e_pipe_fork("fork");
	if (pid == 0)
	{
		cmd_path = find_cmd_path(tools, one_cmd->cmds[0]);
		if (!cmd_path)
			_exit(e_cmd_not_found(one_cmd->cmds[0]));
		tools->envp = env_list_to_array(tools->env_list);
		if (execve(cmd_path, one_cmd->cmds, tools->envp) == -1)
			_exit(e_cmd_not_found(one_cmd->cmds[0]));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
}
