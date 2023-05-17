/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mini_cd.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/09 18:50:09 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/09 18:50:09 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"
#include "executor.h"

int	mini_cd(t_tools *tools, char **simple_cmd)
{
	char	*path;

	path = NULL;
	// char	curdir[PATH_MAX];
	printf("simple_cmd[1]=%s\n", simple_cmd[1]);
	// if (getcwd(curdir, sizeof(curdir)))
	// 	*curdir = '\0';
	if (simple_cmd[1] == NULL || simple_cmd[1][0] == '\0')
		path = getenv("HOME");
	else if (!ft_strncmp(simple_cmd[1], "-", 1)) //TO BE CHECKED IN LOOP
	{
		if (tools->old_pwd[0] == '\0')
			return (1);
		*simple_cmd = tools->old_pwd;
	}
	else if (simple_cmd[1][0] == '~')
	{
		if (simple_cmd[1][1] == '\0')
		{
			tools->old_pwd = getcwd(NULL, sizeof(PATH_MAX));
			path = getenv("HOME");
		}
		else
		{
			tools->old_pwd = getcwd(NULL, sizeof(PATH_MAX));
			path = getenv("HOME");
			simple_cmd[1] = ft_substr(simple_cmd[1], 1, ft_strlen(simple_cmd[1])
					- 1);
			ft_strlcat(path, simple_cmd[1], ft_strlen(simple_cmd[1])
				+ ft_strlen(path) + 1);
		}
	}
	else
	{
		path = simple_cmd[1];
		// chdir(path);
		// tools->pwd = getcwd(NULL, sizeof(PATH_MAX));
	}
	tools->old_pwd = getcwd(NULL, sizeof(PATH_MAX));
	printf("check_path=%s\n", path);
	chdir(path);
	tools->pwd = getcwd(NULL, sizeof(PATH_MAX));
	printf("OLDPWD =%s\n", tools->old_pwd);
	printf("PWD =%s\n", tools->pwd);
	return (0);
}
