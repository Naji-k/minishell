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
	t_env	*env_list;
	char	*expanded_arg;

	env_list = tools->env_list;

	while (env_list)
	{
		if (ft_strncmp((string + 1), env_list->key,
				(ft_strlen(string) - 1)) == 0)
		{
			expanded_arg = ft_strdup(env_list->value);
			if (!expanded_arg)
				exit(EXIT_FAILURE);
			return (expanded_arg);
		}
		env_list = env_list->next;
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
	Helper function which performs actual expansion of $ARG -> ARG_VALUE.
*/
t_token	*handle_expansion(t_token **lst_tokens, t_token *node, t_tools *tools)
{
	char	*expanded_arg;

	expanded_arg = expand_arg(node->cmd, tools);
	printf("Looking to expand: %s\n", node->cmd);
	if (!expanded_arg)
	{
		printf("%s not in env, deleting node\n", node->cmd);
		delete_node(lst_tokens, node);
		node = *lst_tokens;
		return (node);
	}
	else
	{
		printf("Expanding %s to %s\n", node->cmd, expanded_arg);
		free(node->cmd);
		node->cmd = expanded_arg;
		return (node);
	}
}

/*
	Loops through token list and checks if first letter of command is '$'.
	If it is, calls the expand_arg function which will return the relevant
	arg in the env referred to as the "expanded_arg".
	Frees old command and sets new expanded_arg as the new command in token list.
	If cannot find it, it deletes that node from the token list and resets node.
	NOTE: $$ should equal ppid, but currently only gets current pid.
*/
void	expander(t_token **lst_tokens, t_tools *tools)
{
	t_token	*node;

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
			else if (node->cmd[1] == is_whitespace(node->cmd[1])
				|| node->cmd[1] == '\0')
				;
			else
				node = handle_expansion(lst_tokens, node, tools);
		}
		node = node->next;
	}
}
