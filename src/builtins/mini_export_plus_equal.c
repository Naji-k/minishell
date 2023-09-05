/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_export_plus_equal.c                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/09/04 16:26:35 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/09/04 16:26:35 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

char	*remove_plus_equal(char *simple_cmd)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = ft_calloc(sizeof(char), ft_strlen(simple_cmd));
	if (!str)
		return (malloc_error(NULL), NULL);
	while (simple_cmd[i] != '+')
	{
		str[j] = simple_cmd[i];
		i++;
		j++;
	}
	if (simple_cmd[i] == '+')
		i++;
	while (simple_cmd[i])
	{
		str[j] = simple_cmd[i];
		i++;
		j++;
	}
	return (str);
}

/**
 * @brief modify exist_node (plus_equal), 
 * if it has value (join old value with new),
 * else modify the env_exist_node
 * @param exist_node 
 * @param key_value 
 * @return int (SUCCESS || ERROR)
 */
int	export_addto_exist_node(t_env *exist_node, char **key_value)
{
	char	*tmp;
	int		len_node_value;
	int		len_value;

	len_node_value = 0;
	len_value = 0;
	tmp = NULL;
	if (exist_node->has_value)
	{
		len_node_value = ft_strlen(exist_node->value);
		len_value = ft_strlen(key_value[1]);
		tmp = ft_calloc(sizeof(char), (len_node_value + len_value + 2));
		if (!tmp)
			return (malloc_error(NULL), ERROR);
		ft_strlcpy(tmp, exist_node->value, len_node_value + 1);
		if (key_value[1])
			ft_strlcat(tmp, key_value[1], (len_node_value + len_value + 1));
		env_update_key_value(exist_node, NULL, tmp);
		free(tmp);
	}
	else
		env_update_key_value(exist_node, key_value[0], key_value[1]);
	return (SUCCESS);
}

/**
 * @brief the main fn for plus_equal, will handle the simple_cmd(remove '+')
 * then split the args to char **key_value
 * 
 * @param tools 
 * @param simple_command 
 * @return int (SUCCESS || ERROR)
 */
int	export_plus_equal(t_tools *tools, char *simple_command)
{
	char	**key_value;
	char	*valid_arg;
	t_env	*exist_node;

	valid_arg = NULL;
	valid_arg = handle_export_args(simple_command, true);
	if (!valid_arg)
		return (ERROR);
	key_value = split_export_args(valid_arg);
	if (!key_value)
		return (malloc_error(valid_arg), ERROR);
	exist_node = find_env_by_key(tools->env_list, key_value[0]);
	if (exist_node)
	{
		if (export_addto_exist_node(exist_node, key_value) == ERROR)
		{
			free(valid_arg);
			free_2d_arr(key_value);
			return (ERROR);
		}
	}
	else
		create_env_back(tools->env_list, key_value, NULL);
	free_2d_arr(key_value);
	return (SUCCESS);
}
