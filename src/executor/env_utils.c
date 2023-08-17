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

void	init_oldpwd(t_env **env_list)
{
	t_env	*tmp;

	tmp = find_env_by_key(env_list, "OLDPWD");
	if (tmp)
	{
		env_update_key_value(tmp, "OLDPWD", NULL);
	}
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

t_env	*find_env_by_key(t_env **env_list, char *key)
{
	t_env	*env;

	env = (*env_list);
	while (env)
	{
		if (key[ft_strlen(key)] == '=')
		{
			if ((ft_strncmp(env->key, key, ft_strlen(key) - 1) == 0))
				return (env);
		}
		else
		{
			if ((ft_strncmp(env->key, key, ft_strlen(key) - 1) == 0))
				return (env);
		}
		env = env->next;
	}
	return (NULL);
}
