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

void	handle_spaces_expansion(t_token **token_head, t_token *node)
{
	char	**split_string;
	int		i;

	i = 1;
	if (check_quotations(node) == true)
		return ;
	split_string = ft_split(node->cmd, ' ');
	if (!split_string)
		exit(EXIT_FAILURE);
	if (split_string[0] == NULL)
		return (free(split_string));
	free(node->cmd);
	node->cmd = ft_strdup(split_string[0]);
	while (*split_string && split_string[i] != NULL)
	{
		if (split_string[i][0] == '\0')
			;
		else
			create_node(token_head, split_string[i],
				0, ft_strlen(split_string[i]));
		i++;
	}
	free_2d_arr(split_string);
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
	new_string = malloc(sizeof(char) * (ft_strlen(string) * 2));
	if (!new_string)
		exit(EXIT_FAILURE);
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
	free(string);
	return (new_string);
}
