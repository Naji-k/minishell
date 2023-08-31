/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   utils.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/13 09:07:16 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/04/13 09:07:22 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_leaks(void)
{
	system("leaks -q --list minishell");
}

void	malloc_error(void *arg)
{

	free(arg);	g_exit_status = 1;
	printf("Memory allocation failed.\nCommand execution has been halted.\n");
}

void	free_token_list(t_token **lst_head)
{
	t_token	*tmp;
	t_token	*first;

	if (!lst_head)
		return ;
	first = *lst_head;
	if (!first)
		return ;
	tmp = first;
	while (tmp != NULL)
	{
		tmp = tmp->next;
		if (first->cmd)
			free(first->cmd);
		free(first);
		first = tmp;
	}
	*lst_head = NULL;
}

void	free_cmd_list(t_commands **lst_head)
{
	t_commands	*tmp;
	t_commands	*first;

	if (!lst_head)
		return ;
	first = *lst_head;
	tmp = *lst_head;
	if (!first)
		return ;
	while (first != NULL)
	{
		tmp = tmp->next;
		if (first->redirections)
			free(first->redirections);
		if (first->cmds)
			free(first->cmds);
		free(first);
		first = tmp;
	}
	*lst_head = NULL;
}

void	free_2d_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i] != NULL)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
