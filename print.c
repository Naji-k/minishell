/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 10:00:10 by ysrondy           #+#    #+#             */
/*   Updated: 2023/04/21 10:00:17 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_2d_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		printf("%s\n", arr[i]);
		i++;
	}
}


/*
	Prints contents of t_token linked_list.
*/
void	print_token_list(t_token **lst_head)
{
	t_token	*node;

	node = *lst_head;
	if (!node)
		return ;
	while (node)
	{
		printf("Lexer: {%s} | Type: {%d} | Index: {%d}\n", node->cmd, node->type, node->index);
		node = node->next;
	}
}

/*
	Prints contents of t_commands linked_list.
*/
void	print_cmds_list(t_commands **lst_head)
{
	t_commands	*node;
	int			i;

	i = 0;
	node = *lst_head;
	while (node)
	{
		while (node->cmds[i] != NULL)
		{
			printf("Cmds: {%s}\n", node->cmds[i]);
			i++;
		}
		if (node->redirections)
			printf("Redirection: {%s}\n", node->redirections->cmd);
		else
			printf("Redirection: {%p}.\n", node->redirections);
		printf("Builtin: {%s}\n", node->builtin);
		node = node->next;
		i = 0;
	}
}
