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

void	set_bool_quotes(char *string, int i,
	bool *double_quote, bool *single_quote)
{
	if (string[i] == '"')
	{
		if (*single_quote == true)
			;
		else
			*double_quote = !(*double_quote);
	}
	if (string[i] == '\'')
	{
		if (*double_quote == true)
			;
		else
			*single_quote = !(*single_quote);
	}
}

int	is_inside_quote(char *string, int pos_char)
{
	int		i;
	bool	single_quote;
	bool	double_quote;

	i = 0;
	single_quote = false;
	double_quote = false;
	while (string[i])
	{
		set_bool_quotes(string, i, &double_quote, &single_quote);
		if (i == pos_char)
		{
			if (single_quote == true)
				return (SINGLE_QUOTE);
			else if (double_quote == true)
				return (DOUBLE_QUOTE);
			else
				return (NO_QUOTATION);
		}
		i++;
	}
	return (false);
}

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
