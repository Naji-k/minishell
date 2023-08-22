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

/**
 * @brief this is the Main func for buildins using strcmp
		we can call the builtin functions
 * 
 * @param arg type of the builtin ex: cd,export,env...
 * @param tools 
 * @param simple_cmd the builtin and it's command, cd src/
 * @return returned value from the functions (0, 1)
 */
int	run_builtin(char *arg, t_tools *tools, char **simple_cmd)
{
	if (ft_strncmp(arg, "cd", 3) == 0)
		return (mini_cd(tools, simple_cmd));
	if (ft_strncmp(arg, "env", 4) == 0)
		return (mini_env(tools, simple_cmd));
	if (ft_strncmp(arg, "echo", 5) == 0)
		return (mini_echo(tools, simple_cmd));
	if (ft_strncmp(arg, "pwd", 4) == 0)
		return (mini_pwd(tools, simple_cmd));
	if (ft_strncmp(arg, "export", 7) == 0)
		return (mini_export(tools, simple_cmd));
	if (ft_strncmp(arg, "unset", 6) == 0)
		return (mini_unset(tools, simple_cmd));
	if (ft_strncmp(arg, "exit", 5) == 0)
		return (mini_exit(tools, simple_cmd));
	return (EXIT_FAILURE);
}
