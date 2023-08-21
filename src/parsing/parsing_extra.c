/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_extra.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 19:32:24 by ysrondy           #+#    #+#             */
/*   Updated: 2023/08/17 19:32:25 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_str_add_char(char *str, char c)
{
	char	*new_string;
	int		i;

	new_string = malloc(sizeof(char) * (ft_strlen(str) + 2));
	if (!new_string)
		return (malloc_error(str), NULL);
	i = 0;
	while (str[i])
	{
		new_string[i] = str[i];
		i++;
	}
	new_string[i] = c;
	new_string[i + 1] = '\0';
	if (str)
		free(str);
	return (new_string);
}

int	should_expand(char *line, t_tools *tools, t_token *node, int i)
{
	if (((line[i] == '$' && line[i + 1] != '\0')
			|| (i == 0 && line[i] == '~'
				&& is_inside_quote(line, line[i + 1]) == NO_QUOTATION
				&& (line[i + 1] == '\0' || line[i + 1] == '/')))
		&& is_inside_quote(line, i) != SINGLE_QUOTE
		&& get_prev_node(tools->token_head, node)->type != HEREDOC)
		return (true);
	return (false);
}

char	*add_spaces_if_needed(char *str, int in_quotes, int i, int j)
{
	char	*result;

	result = malloc(sizeof(char) * ((strlen(str) + 1) * 2));
	if (!result)
		return (NULL);
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
	return (result);
}

char	*add_spaces_non_literal(char *str)
{
	int		i;
	int		j;
	int		in_quotes;
	char	*new_string;

	i = 0;
	j = 0;
	in_quotes = 0;
	new_string = add_spaces_if_needed(str, in_quotes, i, j);
	free(str);
	return (new_string);
}
