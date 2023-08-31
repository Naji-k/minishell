/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirection.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/03 18:29:22 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/03 18:29:22 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

/**
 * @brief only for REDIRECTION && A_REDIRECTION
 * 
 * @param redirection 
 * @return int SUCCESS || ERROR depends if dup2 fails or something wrong happen
 */
static int	redirect_output(t_token *redirection)
{
	int	file;

	if (redirection->type == REDIRECTION)
	{
		file = open(redirection->cmd, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (file < 0)
			return (error_file_handling(redirection->cmd));
		return (ft_dup2_check(file, STDOUT_FILENO));
	}
	else if (redirection->type == A_REDIRECTION)
	{
		file = open(redirection->cmd, O_CREAT | O_RDWR | O_APPEND, 0644);
		if (file < 0)
			return (error_file_handling(redirection->cmd));
		return (ft_dup2_check(file, STDOUT_FILENO));
	}
	return (ERROR);
}

/**
 * @brief only for IN_FILE, and HEREDOC redirection
 * 
 * @param redirection 
 * @return SUCCESS || ERROR 
 */
static int	redirect_input(t_token *redirection)
{
	int	file;

	if (redirection->type == IN_FILE)
	{
		file = open(redirection->cmd, O_RDONLY, 0644);
		if (file < 0)
			return (error_file_handling(redirection->cmd));
		return (ft_dup2_check(file, STDIN_FILENO));
	}
	else if (redirection->type == HEREDOC)
	{
		return (redirect_open_hd(redirection));
	}
	return (ERROR);
}

/**
 * @brief to open heredoc file, get the path = /tmp/heredoc_index
 * unlink the path,
 * 
 * @param redirection 
 * @return int SUCCESS || ERROR
 */
int	redirect_open_hd(t_token *redirection)
{
	int		file;
	char	*path;
	char	*path_redirection;

	path = NULL;
	path_redirection = ft_itoa(redirection->index);
	if (!path_redirection)
		return (e_heredoc(redirection->cmd));
	path = ft_strjoin("/tmp/", path_redirection);
	if (!path)
	{
		free(path_redirection);
		return (e_heredoc(redirection->cmd));
	}
	file = open(path, O_RDONLY, 0644);
	unlink(path);
	free(path);
	free(path_redirection);
	if (file < 0)
		return (error_file_handling(redirection->cmd));
	return (ft_dup2_check(file, STDIN_FILENO));
}

/**
 * @brief this is the main function for redirection,
 * if it is a valid token, it will call the type function(input, output)
 * @param cmd it's command[i], which includes t_token *redirection
 * @return return value from sub_functions(redirect_input, redirect_output),
 * (SUCCESS || ERROR)
 */
int	redirection(t_commands *cmd)
{
	t_token	*redirection;
	int		return_val;

	return_val = SUCCESS;
	redirection = cmd->redirections;
	while (redirection && return_val == SUCCESS)
	{
		if (redirection->valid == false)
			return (ERROR);
		if (redirection->type == REDIRECTION
			|| redirection->type == A_REDIRECTION)
			return_val = redirect_output(redirection);
		else if (redirection->type == IN_FILE || redirection->type == HEREDOC)
			return_val = redirect_input(redirection);
		else
			ft_putstr_fd(" Too many file descriptors are active\n",
				STDERR_FILENO);
		redirection = redirection->next;
	}
	if (cmd->cmds[0] == NULL)
		return_val = 1;
	return (return_val);
}

/**
 * @brief duplicate an existing file descriptor
 *  after check (old_fd != new_fd), dup2 and close old_fd
 * and protect dup2 if fails,
 * @param old 
 * @param new 
 */
int	ft_dup2_check(int old, int new)
{
	if (old != new)
	{
		if (dup2(old, new) == ERROR)
		{
			ft_putstr_fd("Minishell: ", STDERR_FILENO);
			if (errno == EBADF)
				ft_putstr_fd(" not valid file descriptor.\n", STDERR_FILENO);
			if (errno == EINTR)
				ft_putstr_fd(" Execution is interrupted by a signal.\n",
					STDERR_FILENO);
			if (errno == EMFILE)
				ft_putstr_fd(" Too many file descriptors are active\n",
					STDERR_FILENO);
			return (ERROR);
		}
		close(old);
	}
	return (SUCCESS);
}
