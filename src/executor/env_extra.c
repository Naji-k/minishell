/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_extra.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/05 02:24:47 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/09/05 02:24:47 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	env_size(t_env *node)
{
	int	i;

	i = 0;
	if (!node)
		return (0);
	while (node->next)
	{
		node = node->next;
		i++;
	}
	return (i + 1);
}

t_env	*get_prev_env(t_env **list, t_env *env_node)
{
	t_env	*first_node;

	first_node = (*list);
	if (!first_node || !env_node)
		return (NULL);
	if (first_node == env_node)
		return (first_node);
	while (first_node->next != env_node)
		first_node = first_node->next;
	return (first_node);
}

/*
	this function will search for specific env[key] and delete it
 */
int	env_del_one(t_env **list, char *key)
{
	t_env	*env;
	t_env	*prev;
	t_env	*temp;

	env = find_env_by_key(list, key);
	if (env)
	{
		temp = env->next;
		if (env == *list)
		{
			(*list) = temp;
			(*list)->next = temp->next;
		}
		else
		{
			prev = get_prev_env(list, env);
			if (temp)
				prev->next = temp;
			else
				prev->next = NULL;
		}
		return (free(env->key), free(env->value), free(env), 0);
	}
	return (1);
}
