/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/04 21:01:02 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/04 21:01:02 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "buildin.h"
#include "executor.h"

/* 
	this is the Main func for buildins using strcmp
	 we can call the builtin functions
 */
int	(*execute_buildin(char *args))(t_tools *tools, char **simple_cmd)
{
	int			i;
	const char	*buildin_func_list[] = {
		"cd",
		"env",
		"echo",
		"pwd",
	};
	static int	(*builtin_func[])(t_tools *tools, char **simple_cmd) = {
		&mini_cd,
		&mini_env,
		&mini_echo,
		&mini_pwd};

	i = 0;
	while (i < (sizeof(buildin_func_list) / sizeof(char *)))
	{
		if (ft_strncmp(args, buildin_func_list[i], ft_strlen(args)) == 0)
			return ((*builtin_func[i]));
		i++;
	}
	return (0);
}

int	mini_env(t_tools *tools, char **simple_cmd)
{
	int	i;

	i = 0;
	(void)simple_cmd;
	while (tools->envp[i])
	{
		ft_putendl_fd(tools->envp[i], STDERR_FILENO);
		i++;
	}
	return (-1);
}
