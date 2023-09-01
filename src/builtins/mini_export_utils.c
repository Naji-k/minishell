/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_export_utils.c                                :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/08/17 16:00:48 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/08/17 16:00:48 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

char	**handle_export_args(char *simple_cmd, bool plus_equal)
{
	char	**key_value;
	char	*str;//protect

	str = NULL;
	if (plus_equal == true)
	{
		key_value = ft_split(simple_cmd, '+');
		if (!key_value)
			return (malloc_error(NULL), NULL);
		key_value[1] = ftp_substr(key_value[1], 0, ft_strlen(key_value[1]));
		str = ft_strjoin(key_value[0], key_value[1]);
		free_2d_arr(key_value);
	}
	else
		str = ft_strdup(simple_cmd);
	if (!str)
		return (malloc_error(NULL), NULL);
	key_value = ft_split(str, '=');
	if (!key_value)
		return (malloc_error(NULL), NULL);
	key_value[0] = ftp_strjoin(key_value[0], "=");
	if (!key_value[0])
		return (malloc_error(NULL), NULL);
	free(str);
	return (key_value);
}

int	export_strjoin_to_value(t_env *exist_node, char **key_value)
{
	char	*tmp;

	tmp = NULL;
	tmp = ft_strdup(exist_node->value);
	if (!tmp)
		return (malloc_error(NULL), ERROR);
	tmp = ftp_strjoin(tmp, key_value[1]);
	if (!tmp)
		return (malloc_error(NULL), ERROR);
	env_update_key_value(exist_node, key_value[0], tmp);
	free(tmp);
	return (SUCCESS);
}

int	export_plus_equal(t_tools *tools, char *simple_command)
{
	char	**key_value;
	t_env	*exist_node;

	key_value = handle_export_args(simple_command, true);
	if (!key_value)
		return (ERROR);
	exist_node = find_env_by_key(tools->env_list, key_value[0]);
	if (exist_node)
	{
		if (exist_node->value != NULL)
		{
			if (export_strjoin_to_value(exist_node, key_value) == ERROR)
			{
				free_2d_arr(key_value);
				return (ERROR);
			}
		}
		else
			env_update_key_value(exist_node, key_value[0], key_value[1]);
	}
	else
		create_env_back(tools->env_list, key_value, NULL);
	free_2d_arr(key_value);
	return (SUCCESS);
}

int	export_create(t_env **env_list, char *simple_cmd)
{
	char	**key_value;
	t_env	*exist_node;

	exist_node = NULL;
	key_value = handle_export_args(simple_cmd, false);
	if (!key_value)
		return (ERROR);
	exist_node = find_env_by_key(env_list, key_value[0]);
	if (exist_node)
		env_update_key_value(exist_node, key_value[0], key_value[1]);
	else
		create_env_back(env_list, NULL, simple_cmd);
	free_2d_arr(key_value);
	return (SUCCESS);
}
