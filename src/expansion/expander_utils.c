/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/18 22:54:30 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/08/18 22:54:30 by ysrondy       ########   odam.nl         */
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
		failed_malloc = NULL;
		tools->success_malloc = false;
	}
}

char	*special_expansion(char **expanded_arg, char *string,
	t_tools *tools)
{
	if (string && string[0] == '$' && string[1] == '\0')
	{
		*expanded_arg = ft_strdup(string);
		set_malloc_fail(*expanded_arg, string, tools);
		return (*expanded_arg);
	}
	if (ft_strncmp(string, "~", 1) == 0)
	{
		*expanded_arg = ft_strdup(getenv("HOME"));
		set_malloc_fail(*expanded_arg, string, tools);
		return (*expanded_arg);
	}
	if (ft_strncmp(string, "$?", 2) == 0)
	{
		*expanded_arg = ft_itoa(g_exit_status);
		set_malloc_fail(*expanded_arg, string, tools);
		return (*expanded_arg);
	}
	if (ft_strncmp(string, "$$", 2) == 0)
	{
		*expanded_arg = ft_strdup("1234");
		set_malloc_fail(*expanded_arg, string, tools);
		return (*expanded_arg);
	}
	return (NULL);
}

char	*search_value_expansion(char *string, int *len,
			t_tools *tools)
{
	t_env	*env_list;
	char	*expanded_arg;

	env_list = *tools->env_list;
	while (env_list)
	{
		if (ft_strncmp((string + 1), env_list->key, *len) == 0
			&& ft_strlen(env_list->key) >= *len && env_list->key[*len] == '=')
		{
			expanded_arg = ft_strdup(env_list->value);
			set_malloc_fail(expanded_arg, string, tools);
			return (expanded_arg);
		}
		env_list = env_list->next;
	}
	return (NULL);
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
	char	*final;
	int		len;

	printf("String to be expanded: %s\n", string);
	expanded_arg = ft_strchr(string, '=');
	len = ft_strlen(string) - 1;
	final = special_expansion(&expanded_arg, string, tools);
	if (expanded_arg != NULL)
		return (final);
	expanded_arg = search_value_expansion(string, &len,
			tools);
	return (expanded_arg);
}
