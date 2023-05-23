/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_env.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/17 15:14:20 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/17 15:14:20 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

int	mini_env(t_tools *tools, char **simple_cmd)
{
	t_env	*env;

	env = tools->env_list;
	if (find_env_by_key(&env, "PATH"))
	{
		if (simple_cmd[1] == NULL)
		{
			while (env)
			{
				if (env->has_value == TRUE)
				{
					ft_putstr_fd(env->key, STDOUT_FILENO);
					ft_putendl_fd(env->value, STDOUT_FILENO);
				}
				env = env->next;
			}
			return (0);
		}
	}
	printf("env: %s: No such file or directory", simple_cmd[1]);
	return (127);
}
