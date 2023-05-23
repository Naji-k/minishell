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

void	init_tools_env(t_env **env_list, char **envp)
{
	int		i;
	t_env	*cur;

	cur = NULL;
	*env_list = NULL;
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
			printf("fail in envp[i]\n");
			return ;
		}
		env_add_back(env_list, cur);
		i++;
	}
	cur = find_env_by_key(env_list, "OLDPWD");
	cur->has_value = false;
}

char	**env_list_to_array(t_env **env_list)
{
	int		i;
	char	**env;
	t_env	*tmp;

	tmp = (*env_list);
	i = env_size(*env_list);
	env = ft_calloc(i + 1, sizeof(char **));
	i = 0;
	while (tmp->next)
	{
		env[i] = ft_strjoin(tmp->key, tmp->value);
		i++;
		tmp = tmp->next;
	}
	env[i] = ft_strjoin(tmp->key, tmp->value);
	env[i + 1] = NULL;
	return (env);
}

char	**get_paths(t_env **env_list)
{
	t_env	*env;
	char	**path_arr;

	env = (*env_list);
	while (env->next && ft_strncmp(env->key, "PATH", 4) != 0)
		env = env->next;
	if (ft_strncmp(env->key, "PATH", 4) == 0)
	{
		path_arr = ft_split(env->value, ':');
		if (!path_arr)
			return (NULL);
		ft_strlcpy(path_arr[0], ft_strchr(path_arr[0], '/'),
			ft_strlen(path_arr[0]) - 4);
		add_bslash_path(path_arr);
		return (path_arr);
	}
	return (NULL);
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
}

t_env	*find_env_by_key(t_env **env_list, char *key)
{
	t_env	*env;

	env = (*env_list);
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}
