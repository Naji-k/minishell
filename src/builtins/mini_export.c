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

/**
 * @brief check if the input is valid
 * 
 * @param export_str export args
 * @return int valid=0, +=2, invalid=1
 */
static int	check_input(char *export_str)
{
	int	i;

	i = 0;
	if (export_str[i])
	{
		if (ft_isalpha(export_str[0]))
		{
			while (ft_isalnum(export_str[i]))
				i++;
			if (export_str[i] == '+' && export_str[i + 1] == '=')
				return (2);
			if (export_str[i] != '\0' && export_str[i] != '=')
				return (1);
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
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(env->key, STDOUT_FILENO);
		if (env->value)
		{
			ft_putstr_fd("\"", STDOUT_FILENO);
			ft_putstr_fd(env->value, STDOUT_FILENO);
			ft_putstr_fd("\"", STDOUT_FILENO);
		}
		ft_putstr_fd("\n", STDOUT_FILENO);
		env = env->next;
	}
	g_exit_status = 0;
	return (EXIT_SUCCESS);
}

/**
 * @brief error happens if the input is not valid
 * 
 * @param simple_cmd 
 */
static void	error_export(char *simple_cmd)
{
	ft_putstr_fd("Minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(simple_cmd, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	g_exit_status = 1;
}

/**
 * @brief the main mini_export function
 * if there is no args =>print_export_env
 * then check_input(1=error, 2=plus_equal, 0=valid_input)
 * 
 * @param tools 
 * @param simple_cmd 
 * @return int g_exit_status
 */
int	mini_export(t_tools *tools, char **simple_cmd)
{
	int	i;
	int	checker;

	if (simple_cmd[1] == NULL)
		return (print_export_env(tools));
	i = 1;
	while (simple_cmd[i] != NULL)
	{
		checker = check_input(simple_cmd[i]);
		if (checker == 1)
			return (error_export(simple_cmd[i]), g_exit_status);
		else if (checker == 2)
		{
			if (export_plus_equal(tools, simple_cmd[i]) == ERROR)
				return (g_exit_status);
		}
		else
		{
			if (export_create(tools->env_list, simple_cmd[i]) == ERROR)
				return (g_exit_status);
		}
		g_exit_status = 0;
		i++;
	}
	return (g_exit_status);
}

void	env_update_key_value(t_env *env_node, char *key, char *value)
{
	if (key)
	{
		if (env_node->key)
			free(env_node->key);
		env_node->key = ft_strdup(key);
		if (!env_node->key)
			malloc_error(key);
	}
	if (value != NULL)
	{
		if (env_node->value)
			free(env_node->value);
		env_node->value = ft_strdup(value);
		if (!env_node->value)
			malloc_error(value);
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
