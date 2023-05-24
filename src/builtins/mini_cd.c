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

/*  This fn just for testing purposes */
void	debug_cd(t_tools *tools, char **simple_cmd, char *path)
{
	printf("simple_cmd[1]=%s\n", simple_cmd[1]);
	printf("check_path=%s\n", path);
	printf("OLDPWD =%s\n", tools->old_pwd);
	printf("PWD =%s\n", tools->pwd);
}

int	mini_cd(t_tools *tools, char **simple_cmd)
{
	char	*path;
	char	*tmp_opwd;

	path = NULL;
	if (simple_cmd[1] == NULL || simple_cmd[1][0] == '\0')
		path = cd_home_dir(tools);
	else if (simple_cmd[1][0] == '-') //TO BE CHECKED IN LOOP
		path = mini_cd_oldpwd(tools);
	else if (simple_cmd[1][0] == '~')
		path = cd_root_dir(simple_cmd[1]);
	else if (simple_cmd[1][0] == '.')
		path = ft_strdup(simple_cmd[1]);
	else //cd src (going to child-directory)
		path = cd_sub_dir(simple_cmd[1]);
	//should protect receiving current DIR
	tmp_opwd = getcwd(NULL, sizeof(PATH_MAX));
	if (!path)
	{
		printf("\nNO_PATH==========\n");
		free(tmp_opwd);
		return (EXIT_FAILURE);
	}
	if (chdir(path) < 0)
	{
		printf("cd: %s: No such file or directory\n", simple_cmd[1]);
		free(path);
		free(tmp_opwd);
		return (EXIT_FAILURE);
	}
	update_pwd_env(tools, tmp_opwd);
	debug_cd(tools, simple_cmd, path); //for testing
	free(path);
	free(tmp_opwd);
	return (0);
}
/* This function will check if PWD
	&& OLDPWD exists in env will update the Value of them */

void	update_pwd_env(t_tools *tools, char *tmp_opwd)
{
	t_env	*pwd;
	t_env	*old_pwd;

	tools->old_pwd = ft_strdup(tmp_opwd);
	free(tools->pwd);
	tools->pwd = getcwd(NULL, sizeof(PATH_MAX));
	pwd = find_env_by_key(&tools->env_list, "PWD");
	if (pwd)
	{
		free(pwd->value);
		pwd->value = ft_strdup(tools->pwd);
	}
	old_pwd = find_env_by_key(&tools->env_list, "OLDPWD");
	if (old_pwd)
	{
		free(old_pwd->value);
		old_pwd->has_value = TRUE;
		old_pwd->value = ft_strdup(tools->old_pwd);
	}
	return ;
}

char	*cd_root_dir(char *simple_cmd)
{
	char	*path;
	char	*sub_cmd;

	path = NULL;
	sub_cmd = NULL;
	if (simple_cmd[1] == '\0')
		path = ft_strdup(getenv("HOME"));
	else
	{
		path = getenv("HOME");
		sub_cmd = ft_substr(simple_cmd, 1, ft_strlen(simple_cmd) - 1);
		path = ft_strjoin(path, sub_cmd);
	}
	free(sub_cmd);
	return (path);
}

char	*mini_cd_oldpwd(t_tools *tools)
{
	char	*path;

	path = NULL;
	if (tools->old_pwd != NULL)
		path = ft_strdup(tools->old_pwd);
	else
	{
		ft_putstr_fd("cd: OLDPWD not set", STDERR_FILENO);
		return (NULL);
	}
	return (path);
}

char	*cd_home_dir(t_tools *tools)
{
	char	*path;

	path = NULL;
	path = expand_arg("$HOME", tools);
	if (!path)
	{
		ft_putstr_fd("cd: HOME not set", STDERR_FILENO);
		return (NULL);
	}
	return (path);
}

char	*cd_sub_dir(char *simple_cmd)
{
	char *path;

	path = getcwd(NULL, sizeof(PATH_MAX));
	ft_strlcat(path, "/", ft_strlen(path) + 2);
	ft_strlcat(path, simple_cmd, ft_strlen(path) + ft_strlen(simple_cmd)
			+ 1);
	return (path);
}
