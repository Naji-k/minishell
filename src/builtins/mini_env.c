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

	env = (*tools->env_list);
	if (find_env_by_key(&env, "PATH"))
	{
		if (simple_cmd[1] == NULL)
		{
			while (env)
			{
				if (env->has_value == true)
				{
					ft_putstr_fd(env->key, STDOUT_FILENO);
					if (env->value)
						ft_putstr_fd(env->value, STDOUT_FILENO);
					ft_putstr_fd("\n", STDOUT_FILENO);
				}
				env = env->next;
			}
			return (0);
		}
	}
	ft_putstr_fd("Minishell: env: No such file or directory\n", STDERR_FILENO);
	return (127);
}
