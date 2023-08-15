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

static int	redirect_output(t_token *redirection)
{
	int	file;

	if (redirection->type == REDIRECTION)
	{
		file = open(redirection->cmd, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (file < 0)
			return (error_file_handling(redirection->cmd));
		ft_dup2_check(file, STDOUT_FILENO);
	}
	else if (redirection->type == A_REDIRECTION)
	{
		file = open(redirection->cmd, O_CREAT | O_RDWR | O_APPEND, 0644);
		if (file < 0)
			return (error_file_handling(redirection->cmd));
		ft_dup2_check(file, STDOUT_FILENO);
	}
	return (SUCCESS);
}

static int	redirect_input(t_token *redirection)
{
	int		file;
	char	*path;
	char	*path_redirection;

	path = NULL;
	if (redirection->type == IN_FILE)
	{
		file = open(redirection->cmd, O_RDONLY, 0644);
		if (file < 0)
			return (error_file_handling(redirection->cmd));
		ft_dup2_check(file, STDIN_FILENO);
	}
	else if (redirection->type == HEREDOC)
	{
		path_redirection = ft_itoa(redirection->index);
		path = ft_strjoin("/tmp/", path_redirection);
		file = open(path, O_RDONLY, 0644);
		unlink(path);
		free(path);
		free(path_redirection);
		if (file < 0)
			return (error_file_handling(redirection->cmd));
		ft_dup2_check(file, STDIN_FILENO);
	}
	return (SUCCESS);
}

int	redirection(t_commands *cmd)
{
	t_token	*redirection;
	int		return_val;
	
	return_val = SUCCESS;
	redirection = cmd->redirections;
	while (redirection)
	{
		if (redirection->valid == false)
		{
			dprintf(2,"valid= %d\n", redirection->valid);
			return(ERROR);
		}
		if (redirection->type == REDIRECTION
			|| redirection->type == A_REDIRECTION)
			return_val = redirect_output(redirection);
		else if (redirection->type == IN_FILE || redirection->type == HEREDOC)
			return_val = redirect_input(redirection);
		else
			dprintf(2, "UNKNOW\n");
		redirection = redirection->next;
	}
	// close(file); // need to close previous files otherwise file leak.
	if (cmd->cmds[0] == NULL)
		return_val = 1;
	return (return_val);
}

void	ft_dup2_check(int old, int new)
{
	if (old != new)
	{
		if (dup2(old, new) == -1)
			ft_putstr_fd("dup2 check = - 1\n", 2);
		close(old);
	}
}
