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
	else if (simple_cmd[1][0] == '-')
		path = mini_cd_oldpwd(tools);
	else 
		path = ft_strdup(simple_cmd[1]);
	if (!path)
		return (EXIT_FAILURE);
	tmp_opwd = getcwd(NULL, sizeof(PATH_MAX));
	if (chdir(path) == ERROR)
	{
		error_file_handling(simple_cmd[1]);
		free(path);
		if (tmp_opwd)
			free(tmp_opwd);
		return (EXIT_FAILURE);
	}
	update_pwd_env(tools, tmp_opwd);
	free(path);
	return (0);
}
/* This function will check if PWD
	&& OLDPWD exists in env will update the Value of them */

void	update_pwd_env(t_tools *tools, char *tmp_opwd)
{
	t_env	*tmp;

	if (tools->pwd || tools->pwd[0] != '\0')
		free(tools->old_pwd);
	tools->old_pwd = ftp_strdup(tmp_opwd);
	if (tools->pwd || tools->pwd[0] != '\0')
		free(tools->pwd);
	tools->pwd = getcwd(NULL, sizeof(PATH_MAX));
	if (!tools->pwd)
	{
		ft_putstr_fd("cd: error retrieving current directory: ", STDERR_FILENO);
		ft_putstr_fd("getcwd: cannot access parent directories\n",
			STDERR_FILENO);
	}
	tmp = find_env_by_key(tools->env_list, "PWD");
	if (tmp && tools->pwd)
	{
		free(tmp->value);
		tmp->value = ft_strdup(tools->pwd);
	}
	tmp = find_env_by_key(tools->env_list, "OLDPWD");
	if (tmp)
	{
		free(tmp->key);
		free(tmp->value);
		tmp->key = ft_strdup("OLDPWD=");
		tmp->has_value = TRUE;
		tmp->value = ft_strdup(tools->old_pwd);
	}
}

char	*mini_cd_oldpwd(t_tools *tools)
{
	char	*path;

	path = NULL;
	if (tools->old_pwd != NULL)
		path = ft_strdup(tools->old_pwd);
	else
	{
		ft_putstr_fd("Minishell: ", STDERR_FILENO);
		ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
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
		ft_putstr_fd("Minishell: ", STDERR_FILENO);
		ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
		return (NULL);
	}
	return (path);
}

char	*cd_sub_dir(char *simple_cmd)
{
	char	*path;

	path = getcwd(NULL, sizeof(PATH_MAX));
	ft_strlcat(path, "/", ft_strlen(path) + 2);
	ft_strlcat(path, simple_cmd, ft_strlen(path) + ft_strlen(simple_cmd) + 1);
	return (path);
}
