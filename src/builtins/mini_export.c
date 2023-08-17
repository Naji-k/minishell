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
	int i = 0;
	if (export_str[i])
	{
		// printf("cmd | %s\n", export_str);
		if (ft_isalpha(export_str[0]))
		{
			while (ft_isalnum(export_str[i]))
				// export_str++;
				i++;
			if (export_str[i] == '+' && export_str[i + 1] == '=')
				return (2);
			if (export_str[i] != 0 && export_str[i] != '=')
			{
				printf("CATCHED=%c\n", *export_str);
				return (1);
			}
			else if (export_str[i] == '\0')
				return (0);
			return (0);
		}
		else
			return (EXIT_FAILURE);
	}
	return (1);
}

static int	print_export_env(t_tools *tools)
{
	t_env	*env;

	env = *tools->env_list;
	while (env)
	{
		// if (env->has_value == TRUE)
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
	// exit(1);
	g_exit_status = 0;
	return (EXIT_SUCCESS);
}

static void	error_export(char *simple_cmd)
{
	ft_putstr_fd("Minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(simple_cmd, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	g_exit_status = 1;
}
char	*plus_equal(char *simple_cmd)
{
	char	**key_value;
	char	*str;

	key_value = ft_split(simple_cmd, '+');
	key_value[1] = ftp_substr(key_value[1], 0, ft_strlen(key_value[1]));
	str = ft_strjoin(key_value[0], key_value[1]);
	free_2d_arr(key_value);
	return (str);
}
int	mini_export(t_tools *tools, char **simple_cmd)
{
	int		i;
	char	*tmp;


	tmp = NULL;
	if (simple_cmd[1] == NULL)
		return (print_export_env(tools));
	i = 1;
	while (simple_cmd[i] != NULL)
	{
		printf("cmd[1]=%s\tcmd[2]=%s\n", simple_cmd[1], simple_cmd[2]);
		if (check_input(simple_cmd[i]) != 1)
		{
			if (check_input(simple_cmd[i]) == 2)
			{
				tmp = plus_equal(simple_cmd[i]);
				printf("returned tmp=%s\n", tmp);
				modify_env_value(tools->env_list, tmp, true);
				free(tmp);
			}
			else
			{
				modify_env_value(tools->env_list, simple_cmd[i], false);
			}
			g_exit_status = 0;
		}
		else
			error_export(simple_cmd[i]);
		i++;
	}
	return (g_exit_status);
}

t_env	*modify_env_value(t_env **env_list, char *simple_command,
		bool plus_equal)
{
	t_env	*env_node;
	t_env	*exist_node;
	char	**key_value;
	char	*tmp;

	env_node = NULL;
	tmp = NULL;
	key_value = ft_split(simple_command, '=');
	key_value[0] = ftp_strjoin(key_value[0], "=");
	exist_node = find_env_by_key(env_list, key_value[0]);
	printf("searching for key=%s\t value=%s\n", key_value[0], key_value[1]);
	if (exist_node)
	{
		if (!plus_equal) //replace the node
			env_update_key_value(exist_node, key_value[0], key_value[1]);
		else
		{ //modify the value
			if (exist_node->value != NULL)
			{
				tmp = ft_strdup(exist_node->value);
				tmp = ftp_strjoin(tmp, key_value[1]);
				env_update_key_value(exist_node, key_value[0], tmp);
				free(tmp);
			}
			else
				env_update_key_value(exist_node, key_value[0], key_value[1]);
		}
	}
	else
	{
		env_node = env_new_node(simple_command);
		env_add_back(env_list, env_node, 1);
	}
	free_2d_arr(key_value);
	return (env_node);
}

void	env_update_key_value(t_env *env_node, char *key, char *value)
{
	if (key)
	{
		if (env_node->key)
			free(env_node->key);
		env_node->key = ft_strdup(key);
	}
	if (value != NULL)
	{
		if (env_node->value)
			free(env_node->value);
		env_node->value = ft_strdup(value);
		env_node->has_value = true;
	}
	else
	{
		if (env_node->value)
			free(env_node->value);
		env_node->value = NULL;
		env_node->has_value = false;
	}
}
