/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_syntax.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/14 09:02:18 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/08/14 09:02:19 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	syntax_redirection(t_token *token)
{
	if (token->type == HEREDOC || token->type == REDIRECTION
		|| token->type == A_REDIRECTION
		|| token->type == IN_FILE)
	{
		if (!token->next || token->next->type == HEREDOC
			|| token->next->type == REDIRECTION
			|| token->next->type == A_REDIRECTION
			|| token->next->type == IN_FILE
			|| token->next->type == PIPE)
		{
			g_exit_status = 258;
			if (!token->next)
				printf("%s 'newline'\n", SYN_ERROR);
			else
				printf("%s `%s'\n", SYN_ERROR, token->next->cmd);
			return (1);
		}
	}
	return (0);
}

int	syntax_pipe(t_token *token)
{
	if (token->type == PIPE)
	{
		if (!token->next
			|| (token->next->type == REDIRECTION && !token->next->next)
			|| (token->next->type == A_REDIRECTION && !token->next->next)
			|| (token->next->type == IN_FILE && !token->next->next)
			|| (token->next->type == PIPE))
		{
			g_exit_status = 258;
			printf("%s `%s'\n", SYN_ERROR, token->cmd);
			return (1);
		}
	}
	return (0);
}

int	syntax_dot(t_token *token)
{
	if (token && token->cmd[0] == '.' && token->cmd[1] == '\0' && !token->next)
	{
		g_exit_status = 2;
		printf("%s", DOT_ERROR);
		return (1);
	}
	return (0);
}

int	ambiguous_redirect(t_token *token, t_tools *tools)
{
	(void)tools;
	if (token->type == REDIRECTION || token->type == A_REDIRECTION
		|| token->type == IN_FILE)
	{
		if (token->next->valid == false)
		{
			g_exit_status = 1;
			printf("Minishell: %s: ambiguous redirect.\n", token->next->cmd);
			return (1);
		}
	}
	return (0);
}

int	handle_syntax_error(t_token **tokens_head, t_tools *tools)
{
	t_token	*token;

	token = *tokens_head;
	if (token != NULL)
	{
		if (syntax_dot(token) == 1)
			return (1);
		if ((*tokens_head)->type == PIPE)
		{
			g_exit_status = 258;
			printf("%s `%s'\n", SYN_ERROR, token->cmd);
			return (1);
		}
		while (token != NULL)
		{
			if (syntax_redirection(token) == 1)
				return (1);
			if (syntax_pipe(token) == 1)
				return (1);
			if (ambiguous_redirect(token, tools) == 1)
				return (0);
			token = token->next;
		}
	}
	return (0);
}
