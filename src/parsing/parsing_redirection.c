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
	t_commands	*first;

	first = *cmds_head;
	while (first)
	{
		free_token_list(&first->redirections);
		first = first->next;
	}
}

void	create_redirection_list(t_commands *node_cmds, t_token *start_node)
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

void	add_node_redirection_list(t_commands *node_cmds, t_token *start_node)
{
	t_token		*l_node;

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

void	handle_redirection(t_commands *node_cmds, t_token *start_node)
{
	if (node_cmds->redirections == NULL)
		create_redirection_list(node_cmds, start_node);
	else
		add_node_redirection_list(node_cmds, start_node);
}
