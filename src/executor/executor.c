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

/* 
	this file going to be the main one for the executor, so from here,
	I would like to make some checks
	ex: 1 cmd, multi cmnds, direction?
	and from here call the specific functions
 */

/* 
	executor: MAIN FUNC for executing part..
 */
int	executor(t_tools *tools, t_commands **cmd_head)
{
	if ((*cmd_head)->next == NULL)
	{
		printf("one cmd\n");
		execute_onc_cmd(tools, cmd_head);
	}
	printf("multi_cmd\n");
	return (0);
}
/*
	Here i am checking if the executor is a local(in project dir) | from env->PATH
 */

char	*check_current_dir(char *cmd)
{
	if (access(cmd, F_OK) == -1)
	{
		ft_putstr_fd("it's not local\n", 2);
		return (NULL);
	}
	return (cmd);
}

/*
	If it is only one cmnd, you don't have to fork... 
	so just execute on the main process
 */
int	execute_onc_cmd(t_tools *tools, t_commands **cmd_head)
{
	char	*cmd_path;

	cmd_path = check_current_dir((*cmd_head)->cmds[0]);
	if (!cmd_path)
		cmd_path = find_cmd_path(tools, (*cmd_head)->cmds);
	if (!cmd_path)
		perror("path not found: ");
	if (cmd_path)
	{
		if (execve(cmd_path, (*cmd_head)->cmds, NULL) == -1)
		{
			perror("execve:\n");
			return (2);
		}
	}
	return (0);
}

/*
	find the path of the command...(if it is not local executable)
 */
char	*find_cmd_path(t_tools *tools, char **cmd)
{
	char	*cmd_path;
	int		i;

	i = -1;
	printf("searching for= %s\n", cmd[0]);
	while (tools->paths[++i])
	{
		cmd_path = ft_strjoin(tools->paths[i], cmd[0]);
		if (access(cmd_path, X_OK) == 0)
		{
			printf("find_path=%s\n", cmd_path);
			return (cmd_path);
		}
		free(cmd_path);
	}
	//should free
	return (NULL);
}
