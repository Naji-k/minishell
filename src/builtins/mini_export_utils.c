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
	char	*str;

	if (plus_equal == true)
	{
		key_value = ft_split(simple_cmd, '+');
		key_value[1] = ftp_substr(key_value[1], 0, ft_strlen(key_value[1]));
		str = ft_strjoin(key_value[0], key_value[1]);
		free_2d_arr(key_value);
	}
	else
		str = ft_strdup(simple_cmd); // this is leaking @naji.
	key_value = ft_split(str, '=');
	key_value[0] = ftp_strjoin(key_value[0], "=");
	free(str); // added this to stop leak, please remove comments if you agree.
	return (key_value);
}

void	export_plus_equal(t_tools *tools, char *simple_command)
{
	char	**key_value;
	t_env	*exist_node;
	char	*tmp;

	tmp = NULL;
	key_value = handle_export_args(simple_command, true);
	printf("returned key:%s\tvalue::%s\n", key_value[0], key_value[1]);
	exist_node = find_env_by_key(tools->env_list, key_value[0]);
	if (exist_node)
	{
		if (exist_node->value != NULL)
		{
			tmp = ft_strdup(exist_node->value);
			tmp = ftp_strjoin(tmp, key_value[1]);
			env_update_key_value(exist_node, key_value[0], tmp);
		}
		else
			env_update_key_value(exist_node, key_value[0], key_value[1]);
	}
	else
		create_env_back(tools->env_list, key_value, NULL);
	free(tmp);
	free_2d_arr(key_value);
}

void	export_create(t_env **env_list, char *simple_cmd)
{
	char	**key_value;
	t_env	*exist_node;

	exist_node = NULL;
	key_value = handle_export_args(simple_cmd, false);
	exist_node = find_env_by_key(env_list, key_value[0]);
	if (exist_node)
	{
		env_update_key_value(exist_node, key_value[0], key_value[1]);
	}
	else
	{
		create_env_back(env_list, NULL, simple_cmd);
	}
	free_2d_arr(key_value);
}
