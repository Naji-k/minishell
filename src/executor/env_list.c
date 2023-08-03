/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_list.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/15 14:17:06 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/15 14:17:06 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

t_env	*env_new_node(char *env)
{
	int		i;
	t_env	*new_node;

	i = 0;
	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->value = NULL;
	while (env[i] != '\0' && env[i] != '=')
		i++;
	if (env[i] == '=')
	{
		new_node->key = ft_substr(env, 0, i + 1);
		new_node->value = ft_substr(env, i + 1, ft_strlen(env) - i);
		new_node->has_value = true;
	}
	if (!new_node->value)
	{
		new_node->key = ft_substr(env, 0, i);
		new_node->has_value = true;
	}
	new_node->next = NULL;
	return (new_node);
}

t_env	*env_last(t_env *node)
{
	if (!node)
		return (NULL);
	while (node->next != NULL)
		node = node->next;
	return (node);
}

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

/* add new env(key=value) to env_list */
void	env_add_back(t_env **list, t_env *new)
{
	t_env	*curr;

	if (*list == NULL)
	{
		*list = new;
		return ;
	}
	curr = env_last(*list);
	curr->next = new;
}

/* 
	this function will search for specific env[key] and delete it
 */
int	env_del_one(t_env **list, char *key)
{
	t_env	*temp;
	t_env	*env;
	int		len;

	len = ft_strlen(key);
	env = (*list);
	while (env->next)
	{
		if (ft_strncmp(env->next->key, key, len) == 0)
		{
			temp = env->next;
			if (temp->next)
				env->next = temp->next;
			else
				env->next = NULL;
			free(temp->key);
			free(temp->value);
			free(temp);
			return (0);
		}
		env = env->next;
	}
	return (1);
}

/* 
print env_list for debugging
 */
void	prinft_env(t_env **list)
{
	t_env	*head;

	head = (*list);
	while (head)
	{
		printf("%s%s\n", head->key, head->value);
		head = head->next;
	}
}
