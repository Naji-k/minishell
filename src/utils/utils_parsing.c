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

int	is_builtin(char *string)
{
	if (ft_strncmp(string, "cd", 2) == 0 && string[2] == '\0')
		return (true);
	if (ft_strncmp(string, "echo", 4) == 0 && string[4] == '\0')
		return (true);
	if (ft_strncmp(string, "env", 3) == 0 && string[3] == '\0')
		return (true);
	if (ft_strncmp(string, "exit", 4) == 0 && string[4] == '\0')
		return (true);
	if (ft_strncmp(string, "export", 6) == 0 && string[6] == '\0')
		return (true);
	if (ft_strncmp(string, "pwd", 3) == 0 && string[3] == '\0')
		return (true);
	if (ft_strncmp(string, "unset", 5) == 0 && string[5] == '\0')
		return (true);
	return (false);
}

int	is_next_non_literal(char *string, int i)
{
	while (string[i] != '\0')
	{
		i++;
		if (string[i] == '\0')
			return (false);
		while (is_whitespace(string[i]))
			i++;
		if (find_token_type(string[i], string[i + 1]) != LITERAL)
			return (true);
		return (false);
	}
	return (true);
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
		return (true);
	else
		return (false);
}
