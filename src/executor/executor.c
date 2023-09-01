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
	signal(SIGINT, SIG_IGN);
	if (fd_i == ERROR || fd_o == ERROR)
	{
		g_exit_status = 1;
		ft_putstr_fd("Minishell: error dup STD_IN, STD_OUT\n", STDERR_FILENO);
		return ;
	}
	if ((*cmd_head)->next == NULL)
		one_cmd_handler(tools, cmd_head);
	if ((*cmd_head)->next != NULL)
		multi_commands_handler(tools, cmd_head);
	ft_dup2_check(fd_i, STDIN_FILENO);
	ft_dup2_check(fd_o, STDOUT_FILENO);
}

void	execve_cmd(t_tools *tools, t_commands **cmd_head)
{
	char	*cmd_path;

	cmd_path = find_cmd_path(tools, (*cmd_head)->cmds[0]);
	if (!cmd_path)
		exit(e_find_path((*cmd_head)->cmds[0]));
	tools->envp = env_list_to_array(tools->env_list);
	if (execve(cmd_path, (*cmd_head)->cmds, tools->envp) == -1)
		exit(e_cmd_not_found((*cmd_head)->cmds[0]));
}

/*
	Here i am checking if the executor is a local(in project dir) | from env->PATH
 */

char	*check_current_dir(char *cmd)
{
	int	i;

	i = 0;
	if (ft_strchr(cmd, '/') || cmd[0] == '.')
		return (cmd);
	return (NULL);
}

/*
	find the path of the command...(if it is not local executable)
 */
char	*find_cmd_path(t_tools *tools, char *cmd)
{
	char	*cmd_path;
	char	*tmp;
	int		i;

	cmd_path = check_current_dir(cmd);
	if (cmd_path)
		return (cmd_path);
	tools->paths = get_paths2(tools->env_list);
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
		if (close(fd[0]) == ERROR)
		{
			ft_putstr_fd("close failed fd[0]\n", STDERR_FILENO);
			return (ERROR);
		}
		if (close(fd[1]) == ERROR)
		{
			ft_putstr_fd("close failed fd[1]\n", STDERR_FILENO);
			return (ERROR);
		}
	}
	if (close(old_fd) == ERROR)
	{
		ft_putstr_fd("close failed old_fd\n", STDERR_FILENO);
		return (ERROR);
	}
	return (SUCCESS);
}
