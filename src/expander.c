/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 09:00:46 by ysrondy           #+#    #+#             */
/*   Updated: 2023/04/25 09:01:07 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
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
	char	**envp;
	char	*expanded_arg;
	int		i;

	i = 0;
	envp = tools->envp;
	while (envp[i])
	{
		if (ft_strncmp((string + 1), envp[i], (ft_strlen(string) - 1)) == 0)
		{
			expanded_arg = ft_substr(envp[i], ft_strlen(string),
					(ft_strlen(envp[i]) - ft_strlen(string)));
			if (!expanded_arg)
				exit(EXIT_FAILURE);
			return (expanded_arg);
		}
		i++;
	}
	return (NULL);
}

/*
	Loops through token list and checks if first letter of command is '$'.
	If it is, calls the expand_arg function which will return the relevant
	arg in the env referred to as the "expanded_arg".
	Frees old command and sets new expanded_arg as the new command in token list.
	If cannot find it, sets cmd = '\0'.
	NOTE: Function does not currently handle $ if it is part of a sentence.
	The function always assumes that a $ will be followed by a variable in env.
	Ex. echo " I love $ so much!"
	This will return: "I love  so much."
	(We need to fix this.)
*/
void	expander(t_token **lst_tokens, t_tools *tools)
{
	t_token	*node;
	char	*expanded_arg;

	node = *lst_tokens;

	while (node)
	{
		if (node->cmd[0] == '$')
		{
			expanded_arg = expand_arg(node->cmd, tools);
			if (!expanded_arg)
				node->cmd[0] = '\0';
			else
			{
				free(node->cmd);
				node->cmd = expanded_arg;
			}
		}
		node = node->next;
	}
}
