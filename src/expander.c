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
	Function which deletes a node from the token list.
*/
void	delete_node(t_token **lst_tokens, t_token *node_to_delete)
{
	t_token	*node;

	node = *lst_tokens;

	while (node)
	{
		if (node->next == node_to_delete)
		{
			node->next = node_to_delete->next;
			if (node_to_delete->next)
				node_to_delete->next->index -= 1;
			free(node_to_delete->cmd);
			free(node_to_delete);
		}
		node = node->next;
	}
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
		// printf("Command: %s\n", node->cmd);
		if (node->cmd[0] == '$')
		{
			if (node->cmd[1] == '$')
			{
				free(node->cmd);
				node->cmd = ft_strdup(ft_itoa(getpid())); // can't use this function
			}
			else if (node->cmd[1] == is_whitespace(node->cmd[1]) || node->cmd[1] == '\0')
				;
			else
			{
				expanded_arg = expand_arg(node->cmd, tools);
				printf("Looking to expand: %s\n", node->cmd);
				if (!expanded_arg)
				{
					printf("Couldn't find arg in env\n");
					delete_node(lst_tokens, node);
				}
				else
				{
					free(node->cmd);
					node->cmd = expanded_arg;
				}
			}
		}
		node = node->next;
	}
}
