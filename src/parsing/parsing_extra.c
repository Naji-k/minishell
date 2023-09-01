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

char	*add_spaces_if_needed(char *str, int i, int j, char *new_string)
{
	while (str[i] != '\0')
	{
		if (is_inside_quote(str, i) == NO_QUOTATION)
		{
			if (str[i] == '>' || str[i] == '<' || str[i] == '|')
			{
				if (is_whitespace(str[i - 1]) == false)
					new_string[j++] = ' ';
				new_string[j++] = str[i++];
				if (str[i] == str[i - 1] && str[i] != '|')
					new_string[j++] = str[i++];
				if (str[i] != '>' && str[i] != '<' && str[i] != '|'
					&& is_whitespace(str[i]) == false)
					new_string[j++] = ' ';
			}
			else
				new_string[j++] = str[i++];
		}
		else
			new_string[j++] = str[i++];
	}
	new_string[j] = '\0';
	return (new_string);
}

char	*add_spaces_non_literal(char *str)
{
	int		i;
	int		j;
	char	*new_string;

	i = 0;
	j = 0;
	new_string = malloc(sizeof(char) * ((ft_strlen(str) + 1) * 2));
	if (!new_string)
		return (NULL);
	new_string = add_spaces_if_needed(str, i, j, new_string);
	free(str);
	return (new_string);
}
