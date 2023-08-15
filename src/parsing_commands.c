/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parsing_commands.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/18 08:09:20 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/04/18 08:09:21 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *string)
{
	if (ft_strncmp(string, "cd", 2) == 0 && string[2] == '\0')
		return (TRUE);
	if (ft_strncmp(string, "echo", 4) == 0 && string[4] == '\0')
		return (TRUE);
	if (ft_strncmp(string, "env", 3) == 0 && string[3] == '\0')
		return (TRUE);
	if (ft_strncmp(string, "exit", 4) == 0 && string[4] == '\0')
		return (TRUE);
	if (ft_strncmp(string, "export", 6) == 0 && string[6] == '\0')
		return (TRUE);
	if (ft_strncmp(string, "pwd", 3) == 0 && string[3] == '\0')
		return (TRUE);
	if (ft_strncmp(string, "unset", 5) == 0 && string[5] == '\0')
		return (TRUE);
	return (FALSE);
}

void	handle_redirection(t_commands *node_cmds, t_token *start_node)
{
	t_token		*l_node;

	if (node_cmds->redirections == NULL)
	{
		node_cmds->redirections = malloc(sizeof(t_token));
		if (!node_cmds->redirections)
			exit(EXIT_FAILURE);
		node_cmds->redirections->cmd = ft_strdup(start_node->next->cmd);
		if (node_cmds->redirections->cmd == NULL)
			exit(EXIT_FAILURE);
		node_cmds->redirections->type = start_node->type;
		node_cmds->redirections->valid = start_node->next->valid;
		node_cmds->redirections->next = NULL;
	}
	else
	{
		l_node = malloc(sizeof(t_token));
		if (!l_node)
			exit(EXIT_FAILURE);
		l_node->cmd = ft_strdup(start_node->next->cmd);
		if (l_node->cmd == NULL)
			exit(EXIT_FAILURE);
		l_node->type = start_node->type;
		l_node->valid = start_node->next->valid;
		l_node->next = NULL;
		add_node_back((void **)&node_cmds->redirections, l_node, TOKEN_LIST);
	}
}

void	create_cmd(t_token *start_node, t_token *target_node,
		t_commands **cmd_head, int s_cmds)
{
	t_commands	*node_cmds;
	int			i;
	int			redirection;

	i = 0;
	redirection = FALSE;
	node_cmds = malloc(sizeof(t_commands));
	if (!node_cmds)
		exit(EXIT_FAILURE);
	node_cmds->cmds = malloc(sizeof(char *) * s_cmds);
	if (!node_cmds->cmds)
		exit(EXIT_FAILURE);
	node_cmds->builtin = NULL; // temporary
	node_cmds->redirections = NULL;
	printf("s_cmds: %d\n", s_cmds);
	while (start_node != target_node)
	{
		if (start_node->type == LITERAL)
		{
			// printf("String inside node: %s\n", start_node->cmd);
			if (redirection == TRUE)
				redirection = FALSE;
			else
			{
				// printf("Node %s is at index %d\n", start_node->cmd, i);
				node_cmds->cmds[i] = start_node->cmd;
				i++;
			}
		}
		if (start_node->type != LITERAL && start_node->type != PIPE)
		{
			handle_redirection(node_cmds, start_node);
			redirection = TRUE;
		}
		if (is_builtin(start_node->cmd))
			node_cmds->builtin = start_node->cmd;
		start_node = start_node->next;
	}
	node_cmds->cmds[i] = NULL;
	node_cmds->next = NULL;
	// printf("Added node: %s\n", node_cmds->cmds[0]);
	add_node_back((void **)cmd_head, node_cmds, CMDS_LIST);
}


int	get_lstsize(t_token *lst)
{
	int	i;

	i = 1;
	if (!lst)
		return (0);
	while (lst->next != NULL)
	{
		lst = lst->next;
		i++;
	}
	return (i);
}

/*
	Logic:
		Loops through token list. If find redirection (>), sets redirection == TRUE.
		Command list (char **) will only include everything before redirection. If redirection == TRUE,
		s_cmds does not increase. If finds pipe, creates command which is similar to ft_substring and creates node.
		Sets redirection back to FALSE after pipe. Once reaches end, creates final command which either
		takes only commands after last pipe or the entire string if no pipes encountered.
		Why +2? Takes into account +1 from the fact that we're always checking node_token->next, and +1 from
		the fact that char** should always end with NULL as last string.
*/
void	parse_cmds(t_token **tokens_head, t_commands **cmd_head)
{
	t_token		*node_token;
	t_token		*start_node;
	int			s_cmds;

	s_cmds = (get_lstsize(*tokens_head) + 1);
	start_node = *tokens_head;
	node_token = *tokens_head;

	if (!node_token)
		return ;
	while (node_token->next != NULL)
	{
		if (node_token->type == PIPE)
		{
			// printf("Creating new command.\n");
			// printf("Start Node: [%s] | Target Node: [%s], Number of commands: %d\n", start_node->cmd, node_token->cmd, s_cmds);
			create_cmd(start_node, node_token, cmd_head, s_cmds);
			start_node = node_token->next;
		}
		node_token = node_token->next;
	}
	if (node_token->type == LITERAL)
	{
		// printf("Creating Final Command\n");
		// printf("Start Node: [%s] | Target Node: [%p]\n", start_node->cmd, node_token->next);
		create_cmd(start_node, node_token->next, cmd_head, s_cmds);
	}
}
