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

/*
	Ex: $USER$$ -> $USER $$
*/
void	handle_arg_then_dollar(char *new_string, char *string, int i, int j)
{
	while (string[i] != '\0')
	{
		if (string[i] == '$' && is_whitespace(string[(i + 1)]) == FALSE
			&& string[(i + 1)] != '$')
		{
			i++;
			j++;
			while (is_whitespace(string[i]) == FALSE && string[i] != '\0')
			{
				if (string[i] == '$')
				{
					new_string[j] = ' ';
					new_string[j + 1] = '$';
					j++;
					break ;
				}
				i++;
				j++;
			}
		}
		else
		{
			i++;
			j++;
		}
	}
	printf("New String v2: %s\n", new_string);
}

int	handle_dbl_dollars(char *new_string, char *string, int i, int j)
{
	while (string[i] != '\0')
	{
		if (string[i] == '$' && string[(i + 1)] == '$'
			&& (!is_whitespace(string[(i + 2)]) && string[(i + 2)] != '\0'))
		{
			new_string[j] = '$';
			new_string[(j + 1)] = '$';
			new_string[(j + 2)] = ' ';
			j += 3;
			i += 1;
		}
		else
		{
			i++;
			j++;
		}
	}
	printf("New String v3 with len of %d: %s\n", j, new_string);
	return (j + 1);
}

void	handle_chars_before_arg(char *new_string, char *string, int i, int j)
{
	while (string[i] != '\0')
	{
		if (string[(i + 1)] == '$' && is_whitespace(string[i]) == FALSE)
		{
			if (string[i] == '\'' || string[i] == '"')
			{
				new_string[j] = ' ';
				new_string[(j + 1)] = string[i];
				j += 2;
				i += 1;
			}
			else
			{
				new_string[j] = string[i];
				new_string[(j + 1)] = ' ';
				j += 2;
				i += 1;
			}
		}
		else
		{
			new_string[j] = string[i];
			j += 1;
			i += 1;
		}
	}
	new_string[j] = '\0';
	printf("New String v1: %s\n", new_string);
}

/*
	Function which adds a whitespace after 2 dollar signs or after $ARG in the input string.
	Ex: $ARG$ = $ARG $.
		echo $$$ = echo $$ $.
	Required to correctly expand.
*/
char	*sep_dollars(char *string)
{
	int		len;
	char	*new_string;

	new_string = malloc(sizeof(char) * (ft_strlen(string) * 2));
	// printf("Old String: %s | len: %d\n", string, ft_strlen(string));
	handle_chars_before_arg(new_string, string, 0, 0);
	handle_arg_then_dollar(new_string, string, 0, 0);
	len = handle_dbl_dollars(new_string, string, 0, 0);
	new_string[len] = '\0';
	free(string);
	// printf("New String: %s | len: %d\n", new_string, ft_strlen(new_string));
	return (new_string);
}
