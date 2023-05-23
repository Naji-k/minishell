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
#include <string.h>
#include <sys/errno.h>

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
	fd_i = dup(STDIN_FILENO);
	fd_o = dup(STDOUT_FILENO);
	tools->envp = env_list_to_array(&tools->env_list);
	if ((*cmd_head)->next == NULL)
	{
		if (!(*cmd_head)->builtin)
		{
			printf("one cmd\n");
			execute_onc_cmd(tools, cmd_head);
		}
		if ((*cmd_head)->builtin)
			execute_builtin((*cmd_head)->cmds[0])(tools, (*cmd_head)->cmds);
	}
	if ((*cmd_head)->next != NULL)
	{
	printf("multi_cmd\n");
	multi_comands(tools, cmd_head);
	}
	dup2(fd_i, STDIN_FILENO);
	dup2(fd_o, STDOUT_FILENO);
}
/* 
	this func will split the commands to two parts: 
	LOOP: on all commands -1 ex:lst command
	and run the last command separately
 */

void	multi_comands(t_tools *tools, t_commands **cmd_head)
{
	t_commands	*node;
	int			in;
	int			fd[2];

	in = STDIN_FILENO;
	node = *cmd_head;
	while (node->next != NULL)
	{
		if (node->cmds != NULL)
		{
			if (pipe(fd) == -1)
				ft_putstr_fd("pipe:\n", 2);
			multi_pipex_process(tools, &node, &in, fd);
			if (dup2(in, STDIN_FILENO) == -1)
				ft_putstr_fd("multi_process\n", 2);
		}
		node = node->next;
	}
	if (node->redirections)
		redirection(node);
	last_cmd(tools, &node);
}

void	multi_pipex_process(t_tools *tools, t_commands **cmd_head, int *in,
		int *fd)
{
	char	*cmd_path;
	pid_t	pid;

	// int		fd[2];
	// int		i;
	// if (pipe(fd) == -1)
	// 	ft_putstr_fd("pipe:\n", 2);
	pid = fork();
	if (pid < 0)
		ft_putstr_fd("fork\n", 2);
	if (pid == 0)
	{
		close(fd[0]);
		ft_dup2_check(fd[1], STDOUT_FILENO);
		if ((*cmd_head)->redirections)
			redirection((*cmd_head));
		// ft_dup2_check(*in, STDIN_FILENO);
		if ((*cmd_head)->builtin)
		{
			// i = execute_builtin((*cmd_head)->cmds[0])(tools,(*cmd_head)->cmds);
			//should return the return value from builtin
			exit(execute_builtin((*cmd_head)->cmds[0])(tools,
					(*cmd_head)->cmds));
		}
		cmd_path = find_cmd_path(tools, (*cmd_head)->cmds);
		if (!cmd_path)
			ft_putstr_fd("from find_path\n", 2);
		if (execve(cmd_path, (*cmd_head)->cmds, tools->envp) == -1)
			ft_putstr_fd("execve:\n", 2);
	}
	close(fd[1]);
	close(*in);
	*in = fd[0];
}
/* 
	After finish the loop on all (commands-1) so now just execute the last command
 */

void	last_cmd(t_tools *tools, t_commands **last_cmd)
{
	char	*cmd_path;
	pid_t	pid;
	int		stat;
	int		i;

	pid = fork();
	if (pid == 0)
	{
		if ((*last_cmd)->builtin)
		{
			// printf("<<<<<Buildin>>>>\n");
			i = execute_builtin((*last_cmd)->cmds[0])(tools, (*last_cmd)->cmds);
			exit(i);
		}
		cmd_path = find_cmd_path(tools, (*last_cmd)->cmds);
		if (!cmd_path)
			perror((*last_cmd)->cmds[0]);
		if (execve(cmd_path, (*last_cmd)->cmds, tools->envp) == -1)
			ft_putstr_fd("execve:\n", 2);
	}
	while (wait(&stat) > 0)
		;
	// wait(NULL);
}
/*
	Here i am checking if the executor is a local(in project dir) | from env->PATH
 */

char	*check_current_dir(char *cmd)
{
	if (access(cmd, F_OK) == -1)
	{
		// ft_putstr_fd("it's not local\n", 2);
		return (NULL);
	}
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

	node = *cmd_head;
	pid = fork();
	if (pid == 0)
	{
		cmd_path = find_cmd_path(tools, node->cmds);
		if (!cmd_path)
			printf(" %s: No such file or directory", node->cmds[0]);
		if (node->redirections)
			redirection(node);
		if (cmd_path)
		{
			if (execve(cmd_path, node->cmds, tools->envp) == -1)
			{
				printf("execve:\n\n");
			}
		}
	}
	wait(&pid);
}

/*
	find the path of the command...(if it is not local executable)
 */
char	*find_cmd_path(t_tools *tools, char **cmd)
{
	char	*cmd_path;
	int		i;

	tools->paths = find_path(tools->envp);
	cmd_path = check_current_dir(cmd[0]);
	if (cmd_path)
		return (cmd_path);
	if (tools->paths)
	{
		i = -1;
		while (tools->paths[++i])
		{
			cmd_path = ft_strjoin(tools->paths[i], cmd[0]);
			if (access(cmd_path, X_OK) == 0)
				return (cmd_path);
			free(cmd_path);
		}
	}
	//should free
	return (NULL);
}
