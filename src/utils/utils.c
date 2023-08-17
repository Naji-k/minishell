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
	// system("lsof -c minishell");
}


/*
	Duplicates and returns a char ** array.
*/
char	**ft_arrdup(char **arr)
{
	int		i;
	char	**new_arr;

	if (!arr)
		return (NULL);
	i = 0;
	while (arr[i] != NULL)
		i++;
	new_arr = malloc(sizeof(char *) * (i + 1));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (arr[i] != NULL)
	{
		new_arr[i] = ft_strdup(arr[i]);
		if (new_arr[i] == NULL)
		{
			free_2d_arr(new_arr);
			return (NULL);
		}
		i++;
	}
	new_arr[i] = NULL;
	return (new_arr);
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
	if (!first)
		return ;
	tmp = first;
	while (tmp != NULL)
	{
		tmp = tmp->next;
		free(first->redirections);
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
