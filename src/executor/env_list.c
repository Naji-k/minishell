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

/**
 * @brief Create a key value object from char*env
 * 
 * @param new_node 
 * @param env 
 * @return true 
 * @return false when malloc fails
 */
bool	create_key_value(t_env *new_node, char *env)
{
	int	i;

	i = 0;
	while (env[i] != '\0')
	{
		if (env[i] == '=' && new_node->has_value == false)
		{
			new_node->key = ft_substr(env, 0, i + 1);
			if (!new_node->key)
				return (malloc_error(NULL), false);
			new_node->value = ft_substr(env, i + 1, ft_strlen(env) - i);
			if (!new_node->value)
				return (malloc_error(NULL), false);
			new_node->has_value = true;
		}
		i++;
	}
	if (new_node->has_value == false)
	{
		new_node->key = ft_substr(env, 0, ft_strlen(env));
		if (!new_node->key)
			return (malloc_error(NULL), false);
		new_node->value = NULL;
	}
	return (true);
}

/**
 * @brief create_new_node, by char *env(one line), split the line
 * to key_value, if there is a value =? has_value = true,
 * 
 * @param env 
 * @return t_env* created new_node
 */
t_env	*env_new_node(char *env)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (malloc_error(NULL), NULL);
	new_node->has_value = false;
	new_node->next = NULL;
	if (create_key_value(new_node, env) == false)
		return (NULL);
	return (new_node);
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

/**
 * @brief Create a new_node and set it at the end of env, by two ways:
 * if the inputs as char **key_value, will create new_node;
 * if key_value == char *env (one line) also will create new_node
 * @param env_list 
 * @param key_value char **key_value
 * @param env 
 */
void	create_env_back(t_env **env_list, char **key_value, char *env)
{
	char	*str;
	t_env	*new_node;
	int		total_len;

	new_node = NULL;
	str = NULL;
	if (env == NULL)
	{
		total_len = ft_strlen(key_value[0]) + ft_strlen(key_value[1]);
		str = ft_calloc(sizeof(char), total_len + 2);
		if (!str)
			return (malloc_error(NULL));
		ft_strlcpy(str, key_value[0], ft_strlen(key_value[0]) + 1);
		if (key_value[1])
			ft_strlcat(str, key_value[1], total_len + 2);
		new_node = env_new_node(str);
		env_add_back(env_list, new_node, 1);
	}
	else
	{
		new_node = env_new_node(env);
		env_add_back(env_list, new_node, 1);
	}
	free(str);
}
