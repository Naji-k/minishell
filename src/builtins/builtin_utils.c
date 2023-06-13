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

#include "builtin.h"
#include "executor.h"

/*
	this is the Main func for buildins using strcmp
	 we can call the builtin functions
 */
int	(*execute_builtin(char *args))(t_tools *tools, char **simple_cmd)
{
	unsigned long	i;
	const char		*builtin_func_list[] = {
		"cd",
		"env",
		"echo",
		"pwd",
		"export",
		"unset",
		"exit"
	};
	static int		(*builtin_func[])(t_tools *tools, char **simple_cmd) = {
		&mini_cd,
		&mini_env,
		&mini_echo,
		&mini_pwd,
		&mini_export,
		&mini_unset,
		&mini_exit};

	i = 0;
	while (i < (sizeof(builtin_func_list) / sizeof(char *)))
	{
		if (ft_strncmp(args, builtin_func_list[i], ft_strlen(args)) == 0)
			return ((*builtin_func[i]));
		i++;
	}
	return (0);
}

