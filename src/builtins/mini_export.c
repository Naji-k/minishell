/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_export.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/09 19:24:31 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/09 19:24:31 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

static int	check_input(char *export_str) //error!
{
	if (*export_str)
	{
		printf("cmd | %s\n", export_str);
		if (ft_isalpha(export_str[0]))
		{
			while (ft_isalnum(*export_str))
				export_str++;
			if (*export_str != 0 && (*export_str != '='))
			{
				printf("CATCHED = %c\n", *export_str);
				return (1);
			}
			else if (*export_str == 0)
			{
				return (0);
			}
			return (0);
		}
		else
		{
			printf("is_NOT_alpha\n");
			return (1);
		}
	}
	return (1);
}

void	print_export_env(t_tools *tools)
{
	t_env	*env;

	env = tools->env_list;
	while (env)
	{
		if (env->has_value == TRUE)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(env->key, STDOUT_FILENO);
			if (env->value)
			{
				ft_putstr_fd("\"", STDOUT_FILENO);
				ft_putstr_fd(env->value, STDOUT_FILENO);
				ft_putstr_fd("\"", STDOUT_FILENO);
			}
			ft_putstr_fd("\n", STDOUT_FILENO);
		}
		env = env->next;
	}
}

int	mini_export(t_tools *tools, char **simple_cmd)
{
	int		i;
	t_env	*env_node;

	(void)simple_cmd;
	env_node = NULL;
	i = 0;
	if (simple_cmd[1] == NULL || simple_cmd[1][0] == '\0')
	{
		print_export_env(tools);
		return (0);
	}
	i = 1;
	while (simple_cmd[i] != NULL)
	{
		if (!check_input(simple_cmd[i]))
		{
			env_node = modify_env_value(&tools->env_list, simple_cmd[i]);
			if (!env_node)
				env_node = env_new_node(simple_cmd[i]);
			printf("key:%s\tvalue:%s\n", env_node->key, env_node->value);
			env_add_back(&tools->env_list, env_node);
		}
		else
		{
			g_exit_status = 1;
			printf("ERROR\n");
		}
		i++;
	}
	return (0);
}

t_env	*modify_env_value(t_env **env_list, char *simple_command)
{
	t_env *env_node;
	char **key_value;

	env_node = NULL;
	key_value = ft_split(simple_command, '=');
	if (find_env_by_key(env_list, key_value[0]))
	{
		env_del_one(env_list, key_value[0]);
		env_node = env_new_node(simple_command);
	}
	free_2d_arr(key_value);
	return (env_node);
}