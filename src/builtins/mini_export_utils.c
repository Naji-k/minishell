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

/**
 * @brief will handle the valid args if there += will remove +
 * 
 * @param simple_cmd 
 * @param plus_equal if true remove + and return valid_arg
 * @return char* simple_cmd (without +)
 */
char	*handle_export_args(char *simple_cmd, bool plus_equal)
{
	char	*str;

	str = NULL;
	if (plus_equal == true)
	{
		str = remove_plus_equal(simple_cmd);
		if (!str)
			return (NULL);
	}
	else
		str = ft_strdup(simple_cmd);
	if (!str)
		return (malloc_error(NULL), NULL);
	return (str);
}

/**
 * @brief split the valid_arg to **key_value
 * if there '=' would be added to key[0], if there is no value
 * key_value[1] = NULL;
 * @param str valid_arg(cleaned from '+');
 * @return char** key_value
 */
char	**split_export_args(char *str)
{
	char	**key_value;
	int		len_key;

	len_key = 0;
	key_value = NULL;
	key_value = ft_calloc(sizeof(char *), 3);
	if (!key_value)
		return (malloc_error(NULL), NULL);
	while (str[len_key] != '=' && str[len_key] != '\0')
		len_key++;
	if (str[len_key] == '=')
		len_key++;
	key_value[0] = ft_substr(str, 0, len_key);
	if (!key_value[0])
		return (malloc_error(NULL), free_2d_arr(key_value), NULL);
	if (ft_strlen(str) > len_key)
	{
		key_value[1] = ft_substr(str, len_key, ft_strlen(str) - len_key + 1);
		if (!key_value[1])
			return (malloc_error(NULL), free_2d_arr(key_value), NULL);
	}
	else
		key_value[1] = NULL;
	return (key_value);
}

/**
 * @brief this function will create a env_variable
 *  if the input is valid
 * 
 * @param env_list 
 * @param simple_cmd 
 * @return int (SUCCESS || ERROR)
 */
int	export_create(t_env **env_list, char *simple_cmd)
{
	char	*handled_arg;
	char	**key_value;
	t_env	*exist_node;

	exist_node = NULL;
	key_value = NULL;
	handled_arg = NULL;
	handled_arg = handle_export_args(simple_cmd, false);
	if (!handled_arg)
		return (ERROR);
	key_value = split_export_args(handled_arg);
	if (!key_value)
		return (malloc_error(handled_arg), ERROR);
	exist_node = find_env_by_key(env_list, key_value[0]);
	if (exist_node)
		env_update_key_value(exist_node, key_value[0], key_value[1]);
	else
		create_env_back(env_list, NULL, handled_arg);
	free(handled_arg);
	free_2d_arr(key_value);
	return (SUCCESS);
}
