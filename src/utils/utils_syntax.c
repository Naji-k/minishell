/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils_syntax.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <marvin@codam.nl>                    +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/23 16:35:52 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/08/23 16:35:53 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_syntax_dbl(char *string, int *success_dbl,
	char **closed_dbl, int *i)
{
	int	j;

	if (is_inside_quote(string, (*i)) == SINGLE_QUOTE)
		;
	else
	{
		*success_dbl = false;
		j = ((*i) + 1);
		while (string[j])
		{
			if (string[j] == '"')
			{
				*success_dbl = true;
				*closed_dbl = &string[j];
				break ;
			}
			j++;
		}
	}
}

void	check_syntax_sgl(char *string, int *success_sgl,
	char **closed_sgl, int *i)
{
	int	j;

	if (is_inside_quote(string, (*i)) == DOUBLE_QUOTE)
		;
	else
	{
		*success_sgl = false;
		j = ((*i) + 1);
		while (string[j])
		{
			if (string[j] == '\'')
			{
				*success_sgl = true;
				*closed_sgl = &string[j];
				break ;
			}
			j++;
		}
	}
}

bool	check_syntax_quotations(char **string)
{
	int		i;
	int		success_dbl;
	int		success_sgl;
	char	*closed_dbl;
	char	*closed_sgl;

	i = -1;
	success_dbl = true;
	success_sgl = true;
	closed_dbl = NULL;
	closed_sgl = NULL;
	while ((*string)[++i])
	{
		if ((*string)[i] == '"' && &(*string)[i] != closed_dbl)
			check_syntax_dbl(*string, &success_dbl, &closed_dbl, &i);
		else if ((*string)[i] == '\'' && &(*string)[i] != closed_sgl)
			check_syntax_sgl(*string, &success_sgl, &closed_sgl, &i);
	}
	if (success_dbl && success_sgl)
		return (true);
	else
	{
		(*string)[0] = '\0';
		return (false);
	}
}
