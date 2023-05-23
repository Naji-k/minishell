/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_unset.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/17 22:38:46 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/17 22:38:46 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

/* SIMPLE-VERSION of mini_unset
	here just delete node form the env_list
	should be checked for edge cases */
int	mini_unset(t_tools *tools, char **simple_cmd)
{
	if (env_del_one(&tools->env_list, simple_cmd[1]) > 0)
	{
		printf("%s NOT FOUND\n", simple_cmd[1]);
		return (1);
	}
	if (ft_strncmp(simple_cmd[1], "OLDPWD", 6) == 0)
		tools->old_pwd = NULL;
	prinft_env(&tools->env_list);
	return (0);
}
