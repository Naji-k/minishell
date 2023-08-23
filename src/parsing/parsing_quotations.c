/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 11:43:49 by ysrondy           #+#    #+#             */
/*   Updated: 2023/05/21 11:43:55 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_spaces_expansion(t_token **token_head, t_token *node)
{
	char	**split_string;
	int		i;

	i = 0;
	if (check_quotations(node) == true)
		return (node->cmd);
	split_string = ft_split(node->cmd, ' ');
	if (!split_string)
		return (malloc_error(NULL), NULL);
	if (split_string[0] == NULL)
		return (free(split_string), node->cmd);
	free(node->cmd);
	node->cmd = ft_strdup(split_string[0]);
	if (!node->cmd)
		return (malloc_error(NULL), free_2d_arr(split_string), NULL);
	while (*split_string && split_string[++i] != NULL)
	{
		if (split_string[i][0] == '\0')
			;
		else
			create_node(token_head, split_string[i],
				0, ft_strlen(split_string[i]));
	}
	free_2d_arr(split_string);
	return (node->cmd);
}

int	handle_dbl_quotes(char *string, int *i)
{
	if (string[*i] == '"')
	{
		if (is_inside_quote(string, *i) == SINGLE_QUOTE)
			return (false);
		else
		{
			(*i) += 1;
			return (true);
		}
	}
	return (false);
}

int	handle_sgl_quotes(char *string, int *i)
{
	if (string[*i] == '\'')
	{
		if (is_inside_quote(string, *i) == DOUBLE_QUOTE)
			return (false);
		else
		{
			(*i) += 1;
			return (true);
		}
	}
	return (false);
}

char	*get_string_quotation(char *string)
{
	char	*new_string;

	if ((string[0] == '"' && string[1] == '"' && string[2] == '\0')
		|| (string[0] == '\'' && string[1] == '\'' && string[2] == '\0'))
	{
		string[0] = '\0';
		return (string);
	}
	new_string = malloc(sizeof(char) * (ft_strlen(string) + 1));
	if (!new_string)
		return (malloc_error(string), NULL);
	return (new_string);
}

char	*handle_quotations(char *string)
{
	char	*new_string;
	int		i;
	int		j;
	int		skipped_dbl;
	int		skipped_sgl;

	i = 0;
	j = 0;
	skipped_dbl = false;
	skipped_sgl = false;
	new_string = get_string_quotation(string);
	if (!new_string)
		return (NULL);
	if (new_string == string)
		return (string);
	while (string[i])
	{
		skipped_dbl = handle_dbl_quotes(string, &i);
		skipped_sgl = handle_sgl_quotes(string, &i);
		new_string[j] = string[i];
		if (skipped_dbl == false && skipped_sgl == false)
			increment_if_not_skipped(new_string, &i, &j);
	}
	new_string[j] = '\0';
	printf("New String: %s\n", new_string);
	return (free(string), new_string);
}
