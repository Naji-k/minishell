/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/18 22:54:30 by ysrondy           #+#    #+#             */
/*   Updated: 2023/08/18 22:54:30 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"
#include "executor.h"

void	set_malloc_fail(char *failed_malloc, char *string_to_free,
	t_tools *tools)
{
	if (!failed_malloc)
	{
		malloc_error(string_to_free);
		tools->success_malloc = false;
	}
}

void	special_expansion(char *expanded_arg, char *string,
	t_tools *tools)
{
	if (string && string[0] == '$' && string[1] == '\0')
	{
		expanded_arg = ft_strdup(string);
		set_malloc_fail(expanded_arg, string, tools);
	}
	if (ft_strncmp(string, "~", 1) == 0)
	{
		expanded_arg = ft_strdup(getenv("HOME"));
		set_malloc_fail(expanded_arg, string, tools);
	}
	if (ft_strncmp(string, "$?", 2) == 0)
	{
		expanded_arg = ft_itoa(g_exit_status);
		set_malloc_fail(expanded_arg, string, tools);
	}
	if (ft_strncmp(string, "$$", 2) == 0)
	{
		expanded_arg = ft_strdup("1234");
		set_malloc_fail(expanded_arg, string, tools);
	}
}

/*
	Compares the string after the '$' with the environment variables.
	If it exists, it substrings and returns everything after the '='
	in the environment variable.
	If it doesn't, returns an empty string (same behaviour happens in bash).
	Ex. ARG=123 (found in env);
	Returns: 123
*/
char	*expand_arg(char *string, t_tools *tools)
{
	char	*expanded_arg;
	int		len;
	int		found_equal_sign;

	printf("String to be expanded: %s\n", string);
	found_equal_sign = false;
	expanded_arg = ft_strchr(string, '=');
	if (expanded_arg == NULL)
		len = ft_strlen(string) - 1;
	else
	{
		len = expanded_arg - string - 1;
		found_equal_sign = true;
	}
	special_expansion(expanded_arg, string, tools);
	expanded_arg = search_value_expansion(string, &len,
			tools, found_equal_sign);
	return (expanded_arg);
}
