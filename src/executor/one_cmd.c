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
	char	**command;
	int		i;

	command = (*cmd_head)->cmds;
	i = 0;
	while (command[i])
	{
		tools->has_pipe = false;
		if ((*cmd_head)->redirections)
		{
			if (redirection((*cmd_head)))
				return ;
		}
		if ((*cmd_head)->builtin && command[i][0] != '\0')
		{
			g_exit_status = run_builtin(command[i], tools, &command[i]);
			return ;
		}
		if (command[i])
		{
			execute_onc_cmd(tools, &command[i]);
			return ;
		}
		i++;
	}
}

/**
 * @brief Executor for one command, find a path, convert env to char **env,
	and execve
 * 
 * @param tools 
 * @param cmd_head includes: **cmd, *redirection,  *builtin
 */
void	execute_onc_cmd(t_tools *tools, char **simple_command)
{
	char	*cmd_path;
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == ERROR)
		e_pipe_fork("fork");
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		cmd_path = find_cmd_path(tools, simple_command[0]);
		if (cmd_path == NULL)
			_exit(e_find_path(simple_command[0]));
		tools->envp = env_list_to_array(tools->env_list);
		if (execve(cmd_path, simple_command, tools->envp) == -1)
			_exit(e_cmd_not_found(simple_command[0]));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	signal_checker(status);
}

void	signal_checker(int status)
{
	if (WIFSIGNALED(status))
	{
		printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
	}
	signal(SIGINT, handler_sigint);
}
