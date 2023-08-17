/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 09:00:46 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/04/25 09:01:07 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"
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
	t_env	*env_list;
	char	*expanded_arg;
	int		len;
	int		found_equal_sign;

	// printf("Trying to expand: %s\n", string);
	if (string[0] == '$' && string[1] == '\0')
		return (ft_strdup(string));
	env_list = *tools->env_list;
	found_equal_sign = FALSE;
	expanded_arg = ft_strchr(string, '=');
	if (expanded_arg == NULL)
		len = ft_strlen(string) - 1;
	else
	{
		printf("This is an export.\n");
		len = expanded_arg - string - 1;
		found_equal_sign = TRUE;
	}
	while (env_list)
	{
		if (ft_strncmp(string, "~", 1) == 0)
			return (ft_strdup(getenv("HOME")));
		if (ft_strncmp(string, "$?", 2) == 0)
			return (ft_itoa(g_exit_status));
		if (ft_strncmp(string, "$$", 2) == 0)
			return (ft_strdup("1234"));
		if (ft_strncmp((string + 1), env_list->key, len) == 0
			&& ft_strlen(env_list->key) >= len && env_list->key[len] == '=')
		{
			if (found_equal_sign == TRUE)
				expanded_arg = ft_strjoin(env_list->value, &string[len + 1]);
			else
				expanded_arg = ft_strdup(env_list->value);
			if (!expanded_arg)
				exit(EXIT_FAILURE);
			return (expanded_arg);
		}
		env_list = env_list->next;
	}
	return (NULL);
}
