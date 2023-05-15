/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_pwd.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/09 18:48:14 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/09 18:48:14 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

int	mini_pwd(t_tools *tools, char **simple_cmd)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("\nCurrent working dir: %s\n", cwd);
	else
		perror("getcwd() error");
	return (0);
}
