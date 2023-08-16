/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 18:41:33 by ysrondy           #+#    #+#             */
/*   Updated: 2023/08/16 18:41:34 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Returns the type of the character.
*/
int	find_token_type(char c, char c_next)
{
	if (c == '|')
		return (PIPE);
	if (c == '<' && c_next == '<')
		return (HEREDOC);
	if (c == '>' && c_next == '>')
		return (A_REDIRECTION);
	if (c == '<')
		return (IN_FILE);
	if (c == '>')
		return (REDIRECTION);
	return (LITERAL);
}

void	increment_if_not_skipped(char *new_string, int *i, int *j)
{
	if (new_string[*j] != '\0')
	{
		(*i) += 1;
		(*j) += 1;
	}
}

int	check_quotations(t_token *node)
{
	int	j;

	j = 0;
	while (node->cmd[j])
	{
		if (node->cmd[j] == '"' || node->cmd[j] == '\'')
			return (true);
		j++;
	}
	return (false);
}
