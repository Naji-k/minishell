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
	// tools->old_pwd = "/Users/nakanoun/42cursus"; //for test
	printf("simple_cmd[1]=%s\n", simple_cmd[1]);
	if (simple_cmd[1] == NULL || simple_cmd[1][0] == '\0')
		path = getenv("HOME");
	else if (!ft_strncmp(simple_cmd[1], "-", 1)) //TO BE CHECKED IN LOOP
	{
		if (tools->old_pwd)
			path = tools->old_pwd;
		else
		{
			ft_putstr_fd("cd: OLDPWD not set", STDERR_FILENO);
			return (0);
		}
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
	else if (simple_cmd[1][0] == '.')
	{
		path = simple_cmd[1];
	}
	tools->old_pwd = getcwd(NULL, sizeof(PATH_MAX));
	printf("check_path=%s\n", path);
	chdir(path);
	tools->pwd = getcwd(NULL, sizeof(PATH_MAX));
	printf("OLDPWD =%s\n", tools->old_pwd);
	printf("PWD =%s\n", tools->pwd);
	update_pwd_env(tools);
	printf("\n==============\n");
	mini_env(tools, NULL);
	return (0);
}

void	update_pwd_env(t_tools *tools)
{
	t_env	*pwd;
	t_env	*old_pwd;
	t_env	*tmp;
	char	*old_pwd_value;

	tmp = NULL;
	pwd = find_env_by_key(&tools->env_list, "PWD");
	pwd->value = ft_strdup(tools->pwd);
	old_pwd = find_env_by_key(&tools->env_list, "OLDPWD");
	if (!old_pwd)
	{
		old_pwd_value = ft_strjoin("OLDPWD=", tools->old_pwd);
		tmp = env_new_node(old_pwd_value);
		env_add_back(&tools->env_list, tmp);
		free(old_pwd_value);
	}
	else
		old_pwd->value = ft_strdup(tools->old_pwd);
	return ;
}
