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

void	handle_cmd_creation(t_token *start_node, t_token *target_node,
	t_commands *node_cmds, int *i)
{
	int	redirection;

	redirection = false;
	while (start_node != target_node)
	{
		if (start_node->type == LITERAL)
		{
			if (redirection == true)
				redirection = false;
			else
			{
				node_cmds->cmds[*i] = start_node->cmd;
				(*i) += 1;
			}
		}
		if (start_node->type != LITERAL && start_node->type != PIPE)
		{
			handle_redirection(node_cmds, start_node);
			redirection = true;
		}
		if (is_builtin(start_node->cmd))
			node_cmds->builtin = start_node->cmd;
		start_node = start_node->next;
	}
}

void	create_cmd(t_token *start_node, t_token *target_node,
		t_commands **cmd_head, int s_cmds)
{
	t_commands	*node_cmds;
	int			i;

	i = 0;
	node_cmds = malloc(sizeof(t_commands));
	if (!node_cmds)
		exit(EXIT_FAILURE);
	node_cmds->cmds = malloc(sizeof(char *) * s_cmds);
	if (!node_cmds->cmds)
		exit(EXIT_FAILURE);
	node_cmds->builtin = NULL;
	node_cmds->redirections = NULL;
	handle_cmd_creation(start_node, target_node, node_cmds, &i);
	node_cmds->cmds[i] = NULL;
	node_cmds->next = NULL;
	add_node_back((void **)cmd_head, node_cmds, CMDS_LIST);
}

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
			create_cmd(start_node, node_token, cmd_head, s_cmds);
			start_node = node_token->next;
		}
		node_token = node_token->next;
	}
	if (node_token->type == LITERAL)
		create_cmd(start_node, node_token->next, cmd_head, s_cmds);
}
