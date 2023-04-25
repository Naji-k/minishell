/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 08:09:20 by ysrondy           #+#    #+#             */
/*   Updated: 2023/04/18 08:09:21 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *string)
{
	if (ft_strncmp(string, "cd", 2) == 0)
		return (TRUE);
	if (ft_strncmp(string, "echo", 4) == 0)
		return (TRUE);
	if (ft_strncmp(string, "env", 3) == 0)
		return (TRUE);
	if (ft_strncmp(string, "exit", 4) == 0)
		return (TRUE);
	if (ft_strncmp(string, "export", 6) == 0)
		return (TRUE);
	if (ft_strncmp(string, "pwd", 3) == 0)
		return (TRUE);
	if (ft_strncmp(string, "unset", 5) == 0)
		return (TRUE);
	return (FALSE);
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

	while (start_node != target_node)
	{
		if (start_node->type == LITERAL)
		{
			if (redirection == TRUE)
				redirection = FALSE;
			else
			{
				// printf("Node %s is %d\n", start_node->cmd, start_node->type);
				node_cmds->cmds[i] = start_node->cmd;
			}
		}
		if (start_node->type == REDIRECTION || start_node->type == A_REDIRECTION)
		{
			node_cmds->redirections = start_node->next;
			redirection = TRUE;
		}
		if (is_builtin(start_node->cmd))
			node_cmds->builtin = start_node->cmd; // temporary
		i++;
		start_node = start_node->next;
	}
	node_cmds->cmds[s_cmds - 1] = NULL;
	node_cmds->next = NULL;
	add_node_back((void **)cmd_head, node_cmds, CMDS_LIST);
}
// [ls] [-l] [|] [echo] [a] [>>] [file]

void	parse_cmds(t_token **tokens_head, t_commands **cmd_head)
{
	t_token		*node_token;
	t_token		*start_node;
	int			s_cmds;
	int			redirection;

	s_cmds = 0;
	start_node = *tokens_head;
	node_token = *tokens_head;
	redirection = 0;

	if (!node_token)
		return ;
	while (node_token->next != NULL)
	{
		if (node_token->next->type != LITERAL && node_token->next->type != PIPE)
			redirection = !redirection;
		if (redirection != TRUE)
			s_cmds++;
		if (node_token->type == PIPE)
		{
			create_cmd(start_node, node_token, cmd_head, s_cmds);
			start_node = node_token->next;
			s_cmds = 0;
		}
		node_token = node_token->next;
	}
	if (node_token->type == LITERAL)
	{
		s_cmds += 2;
		create_cmd(start_node, node_token->next, cmd_head, s_cmds);
	}
}
                // 
