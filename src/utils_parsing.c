/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/16 18:32:37 by ysrondy           #+#    #+#             */
/*   Updated: 2023/08/16 18:32:38 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_next_non_literal(char *string, int i)
{
	while (string[i] != '\0')
	{
		i++;
		if (string[i] == '\0')
			return (FALSE);
		while (is_whitespace(string[i]))
			i++;
		if (find_token_type(string[i], string[i + 1]) != LITERAL)
			return (TRUE);
		return (FALSE);
	}
	return (TRUE);
}

char	*add_spaces_non_literal(char *str)
{
	int		i;
	int		j;
	int		in_quotes;
	char	*result;

	i = 0;
	j = 0;
	in_quotes = 0;
	result = malloc(strlen(str) * 3);
	while (str[i] != '\0')
	{
		if (str[i] == '\"' || str[i] == '\'')
			in_quotes = !in_quotes;
		if (!in_quotes && (str[i] == '>' || str[i] == '<' || str[i] == '|'))
		{
			if (i > 0 && str[i - 1] != ' ')
				result[j++] = ' ';
			result[j++] = str[i++];
			if (str[i] == str[i - 1])
				result[j++] = str[i++];
			if (str[i] != ' ')
				result[j++] = ' ';
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	free(str);
	return (result);
}

int	skip_whitespaces(char *string)
{
	int	i;

	i = 0;
	while (is_whitespace(string[i]))
		i++;
	return (i);
}

int	skip_space_and_return(char *string, int start)
{
	if (is_whitespace(string[start]))
		return (start += 1);
	else
		return (start);
}

int	is_whitespace(char c)
{
	if (c == '\f' || c == ' ' || c == '\n' || c == '\r'
		|| c == '\t' || c == '\v')
		return (TRUE);
	else
		return (FALSE);
}
