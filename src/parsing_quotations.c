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

char	*add_single_quote(char *string)
{
	char	*new_string;
	int		i;
	int		j;

	i = 0;
	j = 1;
	new_string = malloc(ft_strlen(string) + 3);
	new_string[0] = '\'';
	new_string[ft_strlen(string) + 1] = '\'';
	while (string[i])
	{
		new_string[j] = string[i];
		i++;
		j++;
	}
	new_string[ft_strlen(string) + 2] = '\0';
	free(string);
	return (new_string);
}

/*
	If i = 1 -> We are removing first quotation mark from string.
	If i = 2 -> We are removing first and last quotation mark.
*/
char	*substring(t_token *node, int i)
{
	char	*new_string;

	new_string = NULL;
	if (i == 1)
		new_string = ft_substr(node->cmd, 1, ft_strlen(node->cmd));
	else if (i == 2)
		new_string = ft_substr(node->cmd, 1, (ft_strlen(node->cmd) - 2));
	if (!new_string)
		exit(EXIT_FAILURE);
	free(node->cmd);
	return (new_string);
}

char	*handle_quotations(char *string)
{
	char	*new_string;
	int		i;
	int		j;
	int		double_quote;
	int		single_quote;

	i = 0;
	j = 0;
	double_quote = FALSE;
	single_quote = FALSE;
	new_string = malloc(sizeof(char) * (ft_strlen(string) + 1));
	if (!new_string)
		exit(EXIT_FAILURE);
	while (string[i])
	{
		if (string[i] == '$' && (string[i + 1] == '\'' || string[i + 1] == '"'))
			i++;
		if (string[i] == '"' && single_quote == FALSE)
		{
			if (double_quote == TRUE)
				double_quote = FALSE;
			else
				double_quote = TRUE;
			i++;
		}
		if (string[i] == '\'' && double_quote == FALSE)
		{
			if (string[i + 1] == '$')
				i--;
			if (single_quote == TRUE)
				single_quote = FALSE;
			else
				single_quote = TRUE;
			i++;
		}
		new_string[j] = string[i];
		i++;
		j++;
	}
	new_string[j] = '\0';
	free(string);
	return (new_string);
}

