/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/15 15:28:37 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/15 15:28:37 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "minishell.h"

/**
 * @brief to init_env
 * 
 * @param env_list env_list minishell env ,which is in tools, 
 * @param envp 
 */
void	init_tools_env(t_env **env_list, char **envp)
{
	int		i;
	t_env	*cur;

	cur = NULL;
	i = 0;
	if (!envp)
	{
		printf("no envp");
		return ;
	}
	while (envp[i] != NULL)
	{
		cur = env_new_node(envp[i]);
		if (!cur)
		{
			free_env_list(env_list);
			printf("fail in envp[i]\n");
			return ;
		}
		env_add_back(env_list, cur, i);
		i++;
	}
	init_oldpwd(env_list);
}

/**
 * @brief it init_oldpwd at the beginning
 * by updating OLDPWD => OLDPWD (with no value)
 * @param env_list 
 */
void	init_oldpwd(t_env **env_list)
{
	t_env	*tmp;
	int		shlvl;
	char	*shl;

	shl = NULL;
	tmp = find_env_by_key(env_list, "OLDPWD");
	if (tmp)
	{
		tmp->has_value = false;
		env_update_key_value(tmp, "OLDPWD", NULL);
	}
	env_del_one(env_list, "_=");
	tmp = find_env_by_key(env_list, "SHLVL");
	if (tmp)
	{
		shlvl = ft_atoi(tmp->value);
		shlvl++;
		shl = ft_itoa(shlvl);
		if (!shl)
			malloc_error(NULL);
		env_update_key_value(tmp, NULL, shl);
		free(shl);
	}
}

/**
 * @brief convert env_list to char **env
 * used in execve
 * @param env_list 
 * @return char** env || NULL if fails
 */
char	**env_list_to_array(t_env **env_list)
{
	int		i;
	char	**env;
	t_env	*tmp;

	tmp = (*env_list);
	i = env_size(*env_list);
	env = ft_calloc(i + 1, sizeof(char **));
	if (!env)
		return (malloc_error("env"), NULL);
	i = 0;
	while (tmp->next)
	{
		if (tmp->value)
			env[i] = ft_strjoin(tmp->key, tmp->value);
		else
			env[i] = ft_strdup(tmp->key);
		i++;
		tmp = tmp->next;
	}
	if (tmp->value)
		env[i] = ft_strjoin(tmp->key, tmp->value);
	else
		env[i] = ft_strdup(tmp->key);
	env[i + 1] = NULL;
	return (env);
}

void	free_env_list(t_env **env_list)
{
	t_env	*temp;

	while (*env_list)
	{
		temp = (*env_list);
		(*env_list) = (*env_list)->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
	free(*env_list);
	*env_list = NULL;
}

/**
 * @brief loop on all env searching for t_env* by key
 * @param env_list 
 * @param key 
 * @return t_env* exist_env
 */
t_env	*find_env_by_key(t_env **env_list, char *key)
{
	t_env	*env;
	int		i;

	env = (*env_list);
	while (env)
	{
		i = 0;
		while ((env->key[i] == key[i]) && key[i] && env->key[i]
			&& key[i] != '=')
			i++;
		if (env->key[i] == '=' && key[i] == '=')
			return (env);
		if (env->key[i] == '=' && key[i] == '\0')
			return (env);
		if (env->key[i] == '\0' && key[i] == '\0')
			return (env);
		if (env->key[i] == '\0' && key[i] == '=')
			return (env);
		env = env->next;
	}
	return (NULL);
}
