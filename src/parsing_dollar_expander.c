/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_dollars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 23:05:03 by ysrondy           #+#    #+#             */
/*   Updated: 2023/05/17 23:05:04 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_arg_then_dollar(char *new_string, char *string, int *i, int *j)
{
	if (string[*i] == '$' && is_whitespace(string[(*i + 1)]) == FALSE
		&& string[(*i + 1)] != '$')
	{
		new_string[*j] = string[*i];
		*i += 1;
		*j += 1;
		while (is_whitespace(string[*i]) == FALSE && string[*i] != '\0')
		{
			if (string[*i] == '$')
			{
				new_string[*j] = ' ';
				new_string[(*j + 1)] = '$';
				*j += 1;
				break ;
			}
			new_string[*j] = string[*i];
			*i += 1;
			*j += 1;
		}
	}
}

void	handle_dbl_dollars(char *new_string, char *string, int *i, int *j)
{
	if (string[*i] == '$' && string[(*i + 1)] == '$'
		&& (!is_whitespace(string[(*i + 2)]) && string[(*i + 2)] != '\0'))
	{
		new_string[*j] = '$';
		new_string[(*j + 1)] = '$';
		new_string[(*j + 2)] = ' ';
		*j += 3;
		*i += 1;
	}
	else
	{
		new_string[*j] = string[*i];
		*j += 1;
	}
}

/*
	Function which adds a whitespace after 2 dollar signs or after $ARG in the input string.
	Ex: $ARG$ = $ARG $.
		echo $$$ = echo $$ $.
	Required to correctly expand.
	Corrections to be made: shortening it to 25 lines.
*/
char	*sep_dollars(char *string)
{
	int		i;
	int		j;
	char	*new_string;

	new_string = malloc(sizeof(char) * (ft_strlen(string) * 2));
	i = 0;
	j = 0;
	// printf("Old String: %s | len: %d\n", string, ft_strlen(string));
	while (string[i] != '\0')
	{
		handle_arg_then_dollar(new_string, string, &i, &j);
		handle_dbl_dollars(new_string, string, &i, &j);
		i++;
	}
	new_string[j] = '\0';
	// printf("New String: %s | len: %d\n", new_string, ft_strlen(new_string));
	return (new_string);
}
