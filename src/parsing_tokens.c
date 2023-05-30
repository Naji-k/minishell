/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing_tokens.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 07:51:57 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/04/11 07:51:58 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Returns true if the char is a whitespace and false if it is not.
*/
int	is_whitespace(char c)
{
	if (c == '\f' || c == ' ' || c == '\n' || c == '\r'
		|| c == '\t' || c == '\v')
		return (TRUE);
	else
		return (FALSE);
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


char	*add_spaces_non_literal(char *string)
{
	int		i;
	char	*new_string;
	int		j;

	i = 0;
	j = 0;
	new_string = malloc(sizeof(char) * (ft_strlen(string) * 2));
	while (string[i] != '\0')
	{
		if (find_token_type(string[i], string[i + 1]) != LITERAL)
		{
			new_string[j] = ' ';
			if (find_token_type(string[i], string[i + 1]) == A_REDIRECTION
				|| find_token_type(string[i], string[i + 1]) == HEREDOC)
			{
				new_string[j + 1] = string[i];
				new_string[j + 2] = string[i + 1];
				new_string[j + 3] = ' ';
				i += 2;
				j += 4;
			}
			else
			{
				new_string[j + 1] = string[i];
				new_string[j + 2] = ' ';
				j += 3;
				i++;
			}
		}
		new_string[j] = string[i];
		i++;
		j++;
	}
	new_string[j] = '\0';
	free(string);
	return (new_string);

}

int	is_next_non_literal(char *string, int i)
{
	while (string[i] != '\0')
	{
		i++;
		while (is_whitespace(string[i]))
			i++;
		if (find_token_type(string[i], string[i + 1]) != LITERAL)
			return (TRUE);
		return (FALSE);
	}
	return (TRUE);
}

// TODO: Ex: $a (where a="")
// TODO: Fix hey="ls             -l"
// TODO: Fix hey="ls -l"
/*
	Loops through string and creates t_token nodes delimited by a whitespace or the end of the string.
*/
void	parse_input(char *_string, t_token **tokens_head)
{
	int		i;
	int		start;
	int		len;
	char	*string;
	int		equal;
	t_token *node;

	printf("String before: |%s|\n", _string);
	string = handle_quotations(_string);
	printf("String after: |%s|\n", string);
	string = add_spaces_non_literal(string);
	printf("String after after: |%s|\n", string);
	string = sep_dollars(string);
	printf("String final: |%s|\n", string);
	i = skip_whitespaces(string);
	len = skip_whitespaces(string);
	equal = FALSE;
	start = i;
	while (string[i] != '\0')
	{
		if (is_next_non_literal(string, i) == TRUE)
		{
			// printf("equal is now false.\n");
			equal = FALSE;
		}
		if ((is_whitespace(string[i]) == TRUE \
			&& is_whitespace(string[i - 1]) == FALSE \
			&& equal == FALSE) \
			|| (string[i + 1] == '\0' && is_whitespace(string[i]) == FALSE))
		{
			if (((string[i + 1] == '\0' && is_whitespace(string[i]) == FALSE)))
				len++;
			// printf("Creating token node starting from %d for a length of %d.\n", start, len);
			node = create_node(tokens_head, string, start, len);
			start = i + 1;
		}
		if (string[i] == '=')
		{
			// printf("Found equal!\n");
			equal = TRUE;
		}
		// printf("I= %d\n", i);
		start = skip_space_and_return(string, start);
		len++;
		i++;
	}
	free(string);
	(void)(node);
}
