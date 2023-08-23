/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/21 10:00:10 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/04/21 10:00:17 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_2d_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != NULL)
	{
		printf("2D Array: %s\n", arr[i]);
		i++;
	}
}

/*
	Prints contents of t_token linked_list.
*/
void	print_token_list(t_token **lst_head, int print_redirection)
{
	t_token	*node;

	node = *lst_head;
	if (!node)
	{
		printf("Tried Printing Token List. Token List Empty.\n");
		return ;
	}
	while (node)
	{
		if (print_redirection == true && node->next == NULL
			&& node->type != HEREDOC && node->type != IN_FILE
			&& node->type != A_REDIRECTION)
			printf("Final Output Redirection: {%s}\n", node->cmd);
		else if (print_redirection == true && node->next == NULL
			&& node->type == A_REDIRECTION)
			printf("Final Append Redirection: {%s}\n", node->cmd);
		else if (print_redirection == true && node->type == A_REDIRECTION)
			printf("Append Redirection: {%s}\n", node->cmd);
		else if (print_redirection == true && node->type == HEREDOC)
			printf("Heredoc Delimiter: {%s}\n", node->cmd);
		else if (print_redirection == true && node->type == IN_FILE)
			printf("In File: {%s}\n", node->cmd);
		else if (print_redirection == true)
			printf("Redirection: {%s}\n", node->cmd);
		else
			printf("Lexer: {%s} | Type: {%d} | Index: {%d}\n", node->cmd,
			node->type, node->index);
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
		if (node->cmds[0] == NULL)
			printf("Cmds: No Commands.\n");
		if (node->redirections)
			print_token_list(&node->redirections, true);
		else
			printf("Redirection: {%p}.\n", node->redirections);
		printf("Builtin: {%s}\n", node->builtin);
		printf("\n");
		node = node->next;
		i = 0;
	}
}
