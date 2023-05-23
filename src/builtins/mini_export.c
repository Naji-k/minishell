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

static int	check_input(char *export_str)
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
	char	*s;
	char	*tmp;

	s = "\"";
	env = tools->env_list;
	while (env)
	{
		if (env->has_value == TRUE)
		{
			ft_putstr_fd("declare -x ", STDOUT_FILENO);
			ft_putstr_fd(env->key, STDOUT_FILENO);
			if (env->value)
			{
				tmp = ft_strjoin(s, env->value);
				tmp = ft_strjoin(tmp, s);
				ft_putstr_fd(tmp, STDOUT_FILENO);
				free(tmp);
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
			env_node = env_new_node(simple_cmd[i]);
			printf("key:%s\tvalue:%s\n", env_node->key, env_node->value);
			env_add_back(&tools->env_list, env_node);
		}
		else
			printf("ERROR\n");
		i++;
	}
	return (0);
}
