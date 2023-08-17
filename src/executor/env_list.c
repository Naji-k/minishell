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
	new_node->has_value = false;
	new_node->next = NULL;
	while (env[i] != '\0')
	{
		if (env[i] == '=' && new_node->has_value == false)
		{
			new_node->key = ft_substr(env, 0, i + 1);
			new_node->value = ft_substr(env, i + 1, ft_strlen(env) - i);
			new_node->has_value = true;
		}
		i++;
	}
	if (new_node->has_value == false)
	{
		new_node->key = ft_substr(env, 0, ft_strlen(env));
		new_node->value = NULL;
	}
	return (new_node);
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
void	env_add_back(t_env **list, t_env *new, int index)
{
	t_env	*curr;

	if (index == 0)
	{
		*list = new;
		return ;
	}
	curr = *list;
	while (curr->next != NULL)
		curr = curr->next;
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

void	create_env_back(t_env **env_list, char **key_value, char *env)
{
	char	*str;
	t_env	*new_node;

	new_node = NULL;
	str = NULL;
	if (env == NULL)
	{
		if (key_value[1] == NULL)
			str = ft_strdup(key_value[0]);
		else
			str = ft_strjoin(key_value[0], key_value[1]);
		new_node = env_new_node(str);
		env_add_back(env_list, new_node, 1);
	}
	else
	{
		new_node = env_new_node(env);
		env_add_back(env_list, new_node, 1);
	}
}
