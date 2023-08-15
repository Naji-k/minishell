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


void	handle_quotations_expansion(t_token **token_head, t_token *node)
{
	char	**split_string;
	int		i;
	int		j;

	i = 1;
	j = 0;
	while (node->cmd[j])
	{
		if (node->cmd[j] == '"' || node->cmd[j] == '\'')
			return ;
		j++;
	}
	split_string = ft_split(node->cmd, ' ');
	if (!split_string)
		exit(EXIT_FAILURE);
	if (split_string[0] == NULL)
	{
		free(split_string);
		return ;
	}
	// printf("Split 1: %s, Split 2: %s\n", split_string[0], split_string[1]);
	free(node->cmd);
	node->cmd = ft_strdup(split_string[0]);
	while (*split_string && split_string[i] != NULL)
	{
		if (split_string[i][0] == '\0')
			;
		else
			create_node(token_head, split_string[i], 0, ft_strlen(split_string[i]));
		i++;
	}
	free_2d_arr(split_string);
}


/* TODO: Fix export.
hey="ls             -l" should run as "ls -l" and print as "ls -l" when run echo "$hey".
*/


/*
	If "$ARG" then expand normal string.
	If $ARG without quotation, then command needs to be split by white spaces.
	But $ARG in environment should not change, only what is given to executor as a command.

	handle error when token head is not empty but cmds head is empty. = syntax error (<><>)
	fix $unknown - DONE
	fix $$ - check w/ Ruben
*/

/*
	Loops through string and creates t_token nodes delimited by a whitespace or the end of the string.
	readline history
*/
void	parse_input(char *_string, t_token **tokens_head, t_tools *tools)
{
	int		i;
	int		start;
	int		len;
	char	*string;
	bool	quotes;
	t_token	*node;

	// printf("Adding spaces after non literals...\n");
	string = add_spaces_non_literal(_string);
	// printf("Separating dollars...\n");
	// string = sep_dollars(string);
	// printf("String before starting parsing: |%s|\n", string);
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
			node->cmd = expand_heredoc(node, node->cmd, tools);
			printf("String after expansion: |%s|\n", node->cmd);
			handle_quotations_expansion(tokens_head, node);
			printf("String after handling quotations expansion: |%s|\n", node->cmd);
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
