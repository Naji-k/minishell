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
		while (ft_isalnum(*export_str))
			export_str++;
		if (ft_isalnum(*export_str) == 0 && (*export_str != '='))
		{
			printf("CATCHED = %c\n", *export_str);
			return (1);
		}
		// export_str++;
	}
	return (0);
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
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putstr_fd(env->key, STDOUT_FILENO);
		tmp = ft_strjoin(s, env->value);
		tmp = ft_strjoin(tmp, s);
		ft_putendl_fd(tmp, STDOUT_FILENO);
		free(tmp);
		env = env->next;
	}
}
int	mini_export(t_tools *tools, char **simple_cmd)
{
	int		i;
	char	s;
	char	*tmp;

	s = 34;
	(void)tools;
	(void)simple_cmd;
	i = 0;
	if (simple_cmd[1] == NULL || simple_cmd[1][0] == '\0')
	{
		print_export_env(tools);
	}
	else
		i = 1;
	while (simple_cmd[i] != NULL)
	{
		//check input
		if (check_input(simple_cmd[i]) == 1)
			printf("ERROR\n");
		i++;
	}
	return (0);
}
