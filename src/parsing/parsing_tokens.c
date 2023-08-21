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

t_token	*create_token_helper(t_tools *tools, char *string, int start, int len)
{
	t_token	*node;
	char	*success_check;
	int		i;

	i = 0;
	node = create_node(tools->token_head, string, start, len);
	if (!node)
		return (free_token_list(tools->token_head), NULL);
	printf("String after creating node: |%s|\n", node->cmd);
	node->cmd = expand_heredoc(node, node->cmd, tools, &i);
	if (!node->cmd)
		return (free_token_list(tools->token_head), NULL);
	printf("String after expansion: |%s|\n", node->cmd);
	success_check = handle_spaces_expansion(tools->token_head, node);
	if (!success_check)
		return (free_token_list(tools->token_head), NULL);
	printf("String after handling spaces expansion: |%s|\n", node->cmd);
	node->cmd = handle_quotations(node->cmd);
	if (!node->cmd)
		return (free_token_list(tools->token_head), NULL);
	printf("String after handling quotations: |%s|\n", node->cmd);
	return (node);
}

void	create_token(char *string, int i, t_tools *tools, bool quotes)
{
	int		len;
	int		start;
	t_token	*node;

	len = skip_whitespaces(string);
	start = i;
	while (string[i] != '\0')
	{
		if (string[i] == '"' || string[i] == '\'')
			quotes = !quotes;
		if ((is_whitespace(string[i]) == true \
			&& is_whitespace(string[i - 1]) == false && quotes == false) \
			|| (string[i + 1] == '\0' && is_whitespace(string[i]) == false))
		{
			if (string[i + 1] == '\0' && is_whitespace(string[i]) == false)
				len++;
			node = create_token_helper(tools, string, start, len);
			if (!node)
				return ;
			start = i + 1;
		}
		start = skip_space_and_return(string, start);
		len++;
		i++;
	}
}

void	start_parsing(char *_string, t_tools *tools)
{
	int		i;
	char	*string;
	bool	quotes;

	if (!_string)
		return ;
	string = add_spaces_non_literal(_string);
	if (!string)
		return (malloc_error(_string));
	i = skip_whitespaces(string);
	quotes = false;
	create_token(string, i, tools, quotes);
	free(string);
}
