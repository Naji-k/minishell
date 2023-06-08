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

	env_list = tools->env_list;
	found_equal_sign = FALSE;
	expanded_arg = ft_strchr(string, '=');
	if (expanded_arg == NULL)
		len = ft_strlen(string) - 1;
	else
	{
		len = expanded_arg - string;
		found_equal_sign = TRUE;
	}
	while (env_list)
	{
		if (ft_strncmp((string + 1), env_list->key, len) == 0
			&& env_list->key[len] == '=')
		{
			if (found_equal_sign == TRUE)
				expanded_arg = ft_strjoin(env_list->value, &string[len]);
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

/*
	Function which deletes a node from the token list.
*/
void	delete_node(t_token **lst_tokens, t_token *node_to_delete)
{
	t_token	*node;

	node = *lst_tokens;

	if (node == node_to_delete)
	{
		free(node->cmd);
		free(node);
		*lst_tokens = NULL;
		return ;
	}
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
	Function which expands token into char** array and creates a node for each string.
	Then implements string in exactly the right place where the original unexpanded node used to be.
*/
void	expansion_into_token_list(t_token **lst_tokens, t_token *node, char *expanded_arg, int single_quote)
{
	char	**split_expanded_arg;
	int		i;
	t_token	*node_before;
	t_token	*node_after;
	t_token	*new_node;
	t_token	*last_node;

	i = 0;
	split_expanded_arg = ft_split(expanded_arg, ' ');
	if (!split_expanded_arg)
		exit(EXIT_FAILURE);
	node_before = *lst_tokens;
	last_node = *lst_tokens;
	while (last_node->next != NULL)
		last_node = last_node->next;
	while (node_before->next != node && node_before->next != NULL)
		node_before = node_before->next;
	node_after = node->next;
	if (node == *lst_tokens)
		delete_node(lst_tokens, node);
	// printf("Node Before: %p, Last Node: %p, Node After: %p\n", node_before, last_node, node_after);
	while (split_expanded_arg[i] != NULL)
	{
		new_node = create_node(lst_tokens, split_expanded_arg[i], 0,
				ft_strlen(split_expanded_arg[i]));
		// printf("Created New Node: %s\n", new_node->cmd);
		if (i == 0 && new_node != *lst_tokens)
			node_before->next = new_node;
		if (single_quote == ADD_QUOTATION && i == 0)
			new_node->cmd = add_single_quote(new_node->cmd, ADD_QUOTATION_BEGIN);
		i++;
	}
	if (single_quote == ADD_QUOTATION)
		new_node->cmd = add_single_quote(new_node->cmd, ADD_QUOTATION_END);
	new_node->next = node_after;
	if (node != node_before)
		last_node->next = NULL;
	delete_node(lst_tokens, node);
	free_2d_arr(split_expanded_arg);
}

/*
	Helper function which performs actual expansion of $ARG -> ARG_VALUE.
*/
t_token	*handle_expansion(t_token **lst_tokens, t_token *node, t_tools *tools,
	int single_quote)
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
		printf("Expanding %s to %s. (Splits on white spaces.)\n", node->cmd, expanded_arg);
		expansion_into_token_list(lst_tokens, node, expanded_arg, single_quote);
		return (*lst_tokens);
	}
}

void	handle_exit_status(t_token *node)
{
	char	*new_string;

	new_string = ft_strjoin(ft_itoa(g_exit_status), &node->cmd[2]);
	free(node->cmd);
	node->cmd = new_string;
}

void	handle_home_dir(t_token *node, t_tools *tools)
{
	t_env	*env;
	char	*path;
	char	*final_path;

	env = find_env_by_key(&tools->env_list, "HOME");
	if (node->cmd[1] == '/')
	{
		path = ft_strdup(env->value);
		if (!path)
			exit(EXIT_FAILURE);
		final_path = ft_strjoin(path, (&node->cmd[1]));
		if (!final_path)
			exit(EXIT_FAILURE);
		free(path);
		free(node->cmd);
		node->cmd = final_path;
	}
	else if (node->cmd[1] == '\0')
	{
		free(node->cmd);
		node->cmd = ft_strdup(env->value);
		return ;
	}
	else
		return ;
}


/*
	Loops through token list and checks if first letter of command is '$'.
	If it is, calls the expand_arg function which will return the relevant
	arg in the env referred to as the "expanded_arg".
	Frees old command and sets new expanded_arg as the new command in token list.
	If cannot find it, it deletes that node from the token list and resets node.
	NOTE: $$ should equal ppid, but currently only gets current pid.
	TODO: Should split on spaces. $a="ls -l" should be "ls", "-l" instead of
	being expanded to "ls -l".
	TODO: Handle heredoc. Check if token before $ARG is <<, if it is, handle differently.
	if "$ARG" do not expand inside hd. if $ARG, expand inside hd.
	TODO: handle Ex: echo a$PWD'b'. Need to change expander.
	TODO: handle "$ARG" (spaces included like one big string) instead of $ARG.
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
				node->cmd = ft_itoa(getpid()); // can't use this function
			}
			else if (node->cmd[1] == is_whitespace(node->cmd[1])
				|| node->cmd[1] == '\0')
				;
			else if (node->cmd[1] == '?')
				handle_exit_status(node);
			else if (node->index == SKIP)
				;
			else
				node = handle_expansion(lst_tokens, node, tools, NO_QUOTATION);
		}
		else if (node->cmd[0] == '\'' && node->cmd[1] == '$'
			&& node->cmd[ft_strlen(node->cmd) - 1] != '\'')
		{
			printf("Found expandable %s which will not be expanded.\n", node->cmd);
			node->cmd = substring(node, 1);
			node->index = SKIP;
		}
		else if (node->cmd[0] == '\'' && node->cmd[1] == '$'
			&& node->cmd[ft_strlen(node->cmd) - 1] == '\'')
		{
			printf("Expand %s but add single quotation marks.\n", node->cmd);
			node->cmd = substring(node, 2);
			node = handle_expansion(lst_tokens, node, tools, ADD_QUOTATION);
		}
		else if (ft_strncmp(node->cmd, "~", 1) == 0)
			handle_home_dir(node, tools);
		if (node)
			node = node->next;
	}
}
