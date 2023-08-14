/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:02:18 by ysrondy           #+#    #+#             */
/*   Updated: 2023/08/14 09:02:19 by ysrondy          ###   ########.fr       */
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
		if (!token->next || token->next->type != LITERAL)
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
// Need to change the error message. Should print the $VAR.
// problem where echo "hello" > $LOGNAME > $ABC should still give error but also create the first file.

int	ambiguous_redirect(t_token *token, t_tools *tools)
{
	if (token->type == REDIRECTION || token->type == A_REDIRECTION
		|| token->type == IN_FILE)
	{
		if (tools->indexes[token->next->index] == 1)
		{
			g_exit_status = 1;
			printf("Minishell: [$VAR]: ambiguous redirect.\n");
			return (1);
		}
	}
	return (0);
}


int	handle_syntax_error(t_token **tokens_head, t_tools *tools)
{
	t_token		*token;

	token = *tokens_head;

	if (syntax_dot(token) == 1)
		return (1);

	while (token != NULL)
	{
		if (syntax_redirection(token) == 1)
			return (1);
		if (syntax_pipe(token) == 1)
			return (1);
		if (ambiguous_redirect(token, tools) == 1)
			return (1);
		token = token->next;
	}
	return (0);
}
