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
	close(fd_i);
	close(fd_o);
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
	free(cmd_path);
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
