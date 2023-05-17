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

/*
	Loops through string and creates t_token nodes delimited by a whitespace or the end of the string.
*/
void	parse_input(char *_string, t_token **tokens_head)
{
	int		i;
	int		start;
	int		len;
	char	*string;

	string = sep_dollars(_string);
	i = skip_whitespaces(string);
	len = skip_whitespaces(string);
	start = i;
	while (string[i] != '\0')
	{
		if ((is_whitespace(string[i]) == TRUE \
			&& is_whitespace(string[i - 1]) == FALSE) \
			|| (string[i + 1] == '\0' && is_whitespace(string[i]) == FALSE))
		{
			if (((string[i + 1] == '\0' && is_whitespace(string[i]) == FALSE)))
				len++;
			create_node(tokens_head, string, start, len);
			start = i + 1;
		}
		start = skip_space_and_return(string, start);
		len++;
		i++;
	}
	free(string);
}
