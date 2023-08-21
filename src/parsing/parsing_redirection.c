/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 19:14:20 by ysrondy           #+#    #+#             */
/*   Updated: 2023/08/17 19:14:21 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_redirection(t_commands **cmds_head)
{
	t_commands	*cmd_node;
	t_token		*redirection_node;
	t_token		*redirection_node_next;

	cmd_node = *cmds_head;
	while (cmd_node)
	{
		redirection_node = cmd_node->redirections;
		redirection_node_next = cmd_node->redirections;
		while (redirection_node_next)
		{
			redirection_node_next = redirection_node_next->next;
			free(redirection_node->cmd);
			free(redirection_node);
			redirection_node = redirection_node_next;
		}
		if (cmd_node->redirections)
			cmd_node->redirections = NULL;
		cmd_node = cmd_node->next;
	}
}

bool	create_redirection_list(t_commands *node_cmds, t_token *start_node)
{
	node_cmds->redirections = malloc(sizeof(t_token));
	if (!node_cmds->redirections)
		return (malloc_error(NULL), false);
	node_cmds->redirections->cmd = NULL;
	node_cmds->redirections->next = NULL;
	node_cmds->redirections->cmd = ft_strdup(start_node->next->cmd);
	if (node_cmds->redirections->cmd == NULL)
		return (malloc_error(NULL), false);
	node_cmds->redirections->type = start_node->type;
	node_cmds->redirections->valid = start_node->next->valid;
	return (true);
}

bool	add_node_redirection_list(t_commands *node_cmds, t_token *start_node)
{
	t_token		*l_node;

	l_node = malloc(sizeof(t_token));
	if (!l_node)
		return (malloc_error(NULL), false);
	l_node->next = NULL;
	add_node_back((void **)&node_cmds->redirections, l_node, TOKEN_LIST);
	l_node->cmd = ft_strdup(start_node->next->cmd);
	if (l_node->cmd == NULL)
		return (malloc_error(NULL), false);
	l_node->type = start_node->type;
	l_node->valid = start_node->next->valid;
	return (true);
}

bool	handle_redirection(t_commands *node_cmds, t_token *start_node)
{
	bool	success;

	if (node_cmds->redirections == NULL)
		success = create_redirection_list(node_cmds, start_node);
	else
		success = add_node_redirection_list(node_cmds, start_node);
	return (success);
}
