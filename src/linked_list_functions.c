/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/12 08:42:20 by ysrondy           #+#    #+#             */
/*   Updated: 2023/04/12 08:42:21 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
	Returns the last node of a linked list.
*/
void	*last_node(void *lst, t_lst_type type)
{
	t_token		*token;
	t_commands	*cmd;

	if (type == TOKEN_LIST)
	{
		token = lst;
		while (token && token->next)
			token = token->next;
		return (token);
	}
	else if (type == CMDS_LIST)
	{
		cmd = lst;
		while (cmd && cmd->next)
			cmd = cmd->next;
		return (cmd);
	}
	return (NULL);
}

/*
	Adds node to the back of the linked list.
*/
void	add_node_back(void **lst_head, void *node, t_lst_type type)
{
	t_token		*token_tmp;
	t_commands	*cmd_tmp;

	if (type == TOKEN_LIST && node)
	{
		if (*lst_head == NULL)
		{
			*lst_head = node;
			return ;
		}
		token_tmp = last_node(*lst_head, TOKEN_LIST);
		token_tmp->next = node;
	}
	if (type == CMDS_LIST && node)
	{
		if (*lst_head == NULL)
		{
			*lst_head = node;
			return ;
		}
		cmd_tmp = last_node(*lst_head, CMDS_LIST);
		cmd_tmp->next = node;
	}
}

/*
	Creates a t_token node inside the t_token linked list.
*/
void	create_node(t_token **tokens_head, char *string, int start, int len)
{
	t_token		*node;
	static int	i = 0;

	node = malloc(sizeof(t_token) * 1);
	if (!node)
		exit(EXIT_FAILURE);
	// printf("Start Pos: %d, Len %d\n", start, (len - start));
	node->cmd = ft_substr(string, start, (len - start));
	// printf("Created: %s\n", node->cmd);
	node->type = find_token_type(string[start], string[start + 1]);
	node->index = i;
	i++;
	node->next = NULL;
	add_node_back((void **)tokens_head, node, TOKEN_LIST);
}
