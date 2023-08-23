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

void	check_syntax_dbl(char *string, int *success_dbl, char **closed_dbl, int *i)
{
	int	j;

	if (is_inside_quote(string, (*i)) == SINGLE_QUOTE)
		;
	else
	{
		*success_dbl = false;
		j = ((*i) + 1);
		while (string[j])
		{
			if (string[j] == '"')
			{
				*success_dbl = true;
				*closed_dbl = &string[j];
				break ;
			}
			j++;
		}
	}
}

void	check_syntax_sgl(char *string, int *success_sgl, char **closed_sgl, int *i)
{
	int	j;

	if (is_inside_quote(string, (*i)) == DOUBLE_QUOTE)
			;
	else
	{
		*success_sgl = false;
		j = ((*i) + 1);
		while (string[j])
		{
			if (string[j] == '\'')
			{
				*success_sgl = true;
				*closed_sgl = &string[j];
				break ;
			}
			j++;
		}
	}
}

bool	check_syntax_quotations(char *string)
{
	int		i;
	int 	success_dbl;
	int		success_sgl;
	char	*closed_dbl;
	char	*closed_sgl;

	i = 0;
	success_dbl = true;
	success_sgl = true;
	closed_dbl = NULL;
	closed_sgl = NULL;

	while (string[i])
	{
		if (string[i] == '"' && &string[i] != closed_dbl)
			check_syntax_dbl(string, &success_dbl, &closed_dbl, &i);
		else if (string[i] == '\'' && &string[i] != closed_sgl)
			check_syntax_sgl(string, &success_sgl, &closed_sgl, &i);
		i++;
	}
	return (success_dbl && success_sgl);
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
