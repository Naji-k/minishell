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
void	executor(t_tools *tools, t_commands **cmd_head)
{
	if ((*cmd_head)->next == NULL)
	{
		printf("one cmd\n");
		if (!(*cmd_head)->builtin)
			execute_onc_cmd(tools, cmd_head);
	}
	if ((*cmd_head)->next != NULL)
	{
		printf("multi_cmd\n");
		multi_comands(tools, cmd_head);
	}
	// return (0);
}
/* 
	this func will split the commands to two parts: 
	LOOP: on all commands -1 ex:lst command
	and run the last command separately
 */

void	multi_comands(t_tools *tools, t_commands **cmd_head)
{
	int			file;
	t_commands	*node;
	int			in;

	in = STDIN_FILENO;
	node = *cmd_head;
	while (node->next != NULL)
	{
		if (node->builtin)
			node = node->next;
		if (node->cmds != NULL)
		{
			multi_pipex_process(tools, &node, &in);
			if (dup2(in, STDIN_FILENO) == -1)
				ft_putstr_fd("dup2_file\n", 2);
		}
		node = node->next;
	}
	// printf("LAST_cmd=%s\n", node->cmds[0]);
	if (node->redirections)
		redirection(node);
	last_cmd(tools, &node);
}

void	multi_pipex_process(t_tools *tools, t_commands **cmd_head, int *in)
{
	char		*cmd_path;
	int			fd[2];
	pid_t		pid;
	int			file;
	t_commands	*node;

	node = *cmd_head;
	if (pipe(fd) == -1)
		ft_putstr_fd("pipe:\n", 2);
	pid = fork();
	if (pid < 0)
		ft_putstr_fd("fork\n", 2);
	if (pid == 0)
	{
		close(fd[0]);
		if (node->redirections)
			redirection(node);
		else
		{
			ft_dup2_check(fd[1], STDOUT_FILENO);
			// if (dup2(fd[1], STDOUT_FILENO) == -1)
			// 	ft_putstr_fd("dup2 - 1\n", 2);
		}
		ft_dup2_check(*in, STDIN_FILENO);
		// if (dup2(*in, STDIN_FILENO) == -1)
		// 	ft_putstr_fd("dup2 - 1\n", 2);
		cmd_path = find_cmd_path(tools, node->cmds);
		if (!cmd_path)
			ft_putstr_fd("from find_path\n", 2);
		if (execve(cmd_path, node->cmds, NULL) == -1)
			ft_putstr_fd("execve:\n", 2);
	} //parent process
	close(fd[1]);
	close(*in);
	close(file);
	*in = fd[0];
}
/* 
	After finish the loop on all (commands-1) so now just execute the last command
 */
void	last_cmd(t_tools *tools, t_commands **cmd_head)
{
	char	*cmd_path;
	int		fd[2];
	pid_t	pid;
	int		stat;

	pid = fork();
	if (pid == 0)
	{
		cmd_path = find_cmd_path(tools, (*cmd_head)->cmds);
		if (!cmd_path)
			ft_putstr_fd("from find_path\n", 2);
		if (execve(cmd_path, (*cmd_head)->cmds, NULL) == -1)
			ft_putstr_fd("execve:\n", 2);
	}
	while (wait(&stat) > 0)
		;
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
int	execute_onc_cmd(t_tools *tools, t_commands **cmd_head)
{
	char		*cmd_path;
	int			file;
	t_commands	*node;

	node = *cmd_head;
	if (node->redirections)
	{
		redirection(node);
	}
	cmd_path = find_cmd_path(tools, node->cmds);
	if (!cmd_path)
		printf("path not found: \n");
	if (cmd_path)
	{
		if (execve(cmd_path, node->cmds, NULL) == -1)
		{
			printf("execve:\n\n");
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

	cmd_path = check_current_dir(cmd[0]);
	if (cmd_path)
		return (cmd_path);
	i = -1;
	while (tools->paths[++i])
	{
		cmd_path = ft_strjoin(tools->paths[i], cmd[0]);
		if (access(cmd_path, X_OK) == 0)
		{
			// printf("find_path=%s\n", cmd_path);
			// ft_putstr_fd("FOUND path\n", 2);
			return (cmd_path);
		}
		free(cmd_path);
	}
	//should free
	return (NULL);
}
