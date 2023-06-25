/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing_tokens.c                                   :+:      :+:    :+:   */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/11 07:51:57 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/06/01 19:08:22 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"

//TODO: Fix $$.

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

/* TODO: Fix export.
hey="ls             -l" should run as "ls -l" and print as "ls -l" when run echo "$hey".
*/

/*
	Loops through string and creates t_token nodes delimited by a whitespace or the end of the string.
*/
void	parse_input(char *_string, t_token **tokens_head, t_tools *tools)
{
	int		i;
	int		start;
	int		len;
	char	*string;
	bool	quotes;
	t_token *node;

	printf("Adding spaces after non literals...\n");
	string = add_spaces_non_literal(_string);
	// printf("Separating dollars...\n");
	// string = sep_dollars(string);
	printf("String before starting parsing: |%s|\n", string);
	i = skip_whitespaces(string);
	len = skip_whitespaces(string);
	start = i;
	quotes = false;

	while (string[i] != '\0')
	{
		if (string[i] == '"' || string[i] == '\'')
			quotes = !quotes;
		if ((is_whitespace(string[i]) == TRUE \
			&& is_whitespace(string[i - 1]) == FALSE && quotes == false) \
			|| (string[i + 1] == '\0' && is_whitespace(string[i]) == FALSE))
		{
			if (((string[i + 1] == '\0' && is_whitespace(string[i]) == FALSE)))
				len++;
			// printf("Creating token node starting from %d for a length of %d.\n", start, len);
			node = create_node(tokens_head, string, start, len);
			printf("String after creating node: |%s|\n", node->cmd);
			node->cmd = expand_heredoc(node->cmd, tools);
			printf("String after expansion: |%s|\n", node->cmd);
			node->cmd = handle_quotations(node->cmd);
			printf("String after handling quotations: |%s|\n", node->cmd);
			start = i + 1;
		}
		start = skip_space_and_return(string, start);
		len++;
		i++;
	}
	free(string);
	(void)(node);
}
