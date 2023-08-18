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

void	handle_literal(t_token *start_node, t_commands *node_cmds,
	int *i, int *redirection)
{
	if (start_node->type == LITERAL)
	{
		if (*redirection == true)
			*redirection = false;
		else
		{
			node_cmds->cmds[*i] = start_node->cmd;
			(*i) += 1;
		}
	}
}

bool	handle_cmd_creation(t_token *start_node, t_token *target_node,
	t_commands *node_cmds, int *i)
{
	int	redirection;
	int	success;

	redirection = false;
	while (start_node != target_node)
	{
		handle_literal(start_node, node_cmds, i, &redirection);
		if (start_node->type != LITERAL && start_node->type != PIPE)
		{
			success = handle_redirection(node_cmds, start_node);
			if (success == false)
				return (false);
			redirection = true;
		}
		if (is_builtin(start_node->cmd))
			node_cmds->builtin = start_node->cmd;
		start_node = start_node->next;
	}
	return (true);
}

t_commands	*create_cmd(t_token *start_node, t_token *target_node,
		t_commands **cmd_head, int s_cmds)
{
	t_commands	*node_cmds;
	int			i;
	bool		success;

	i = 0;
	node_cmds = malloc(sizeof(t_commands));
	if (!node_cmds)
		return (malloc_error(NULL), NULL);
	node_cmds->next = NULL;
	node_cmds->redirections = NULL;
	add_node_back((void **)cmd_head, node_cmds, CMDS_LIST);
	(void)(s_cmds);
	node_cmds->cmds = malloc(sizeof(char *) * s_cmds);
	if (!(node_cmds->cmds))
		return (malloc_error(NULL), NULL);
	node_cmds->builtin = NULL;
	success = handle_cmd_creation(start_node, target_node, node_cmds, &i);
	if (success == false)
		return (free_redirection(cmd_head), NULL);
	node_cmds->cmds[i] = NULL;
	return (node_cmds);
}

void	create_last_command(t_token *start_node, t_token *node_token,
	t_commands **cmd_head, int s_cmds)
{
	t_commands	*cmd;

	if (node_token->type == LITERAL)
	{
		cmd = create_cmd(start_node, node_token->next, cmd_head, s_cmds);
		if (!cmd)
			return (free_cmd_list(cmd_head));
	}
}

void	parse_cmds(t_token **tokens_head, t_commands **cmd_head)
{
	t_token		*node_token;
	t_token		*start_node;
	t_commands	*cmd;
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
			cmd = create_cmd(start_node, node_token, cmd_head, s_cmds);
			if (!cmd)
				return (free_cmd_list(cmd_head));
			start_node = node_token->next;
		}
		node_token = node_token->next;
	}
	create_last_command(start_node, node_token, cmd_head, s_cmds);
}
