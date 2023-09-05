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

/**
 * @brief the main fn for cd,
 * check if calling the home, or cd -, and changeDir, then update 
 * PWD, OLDPWD
 * @param tools 
 * @param simple_cmd just the command and the args, cd home..
 * @return 0 for success, 1 for failure
 */
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
	return (SUCCESS);
}

/**
 * This function will check if PWD
	&& OLDPWD exists in env will update the Value of them,
	and update the value of them in tools (as backup)
 * 
 * @param tools 
 * @param tmp_opwd oldpwd, comes from getcwd before change dir
 */

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
		env_update_key_value(tmp, NULL, tools->pwd);
	tmp = find_env_by_key(tools->env_list, "OLDPWD");
	if (tmp)
		env_update_key_value(tmp, "OLDPWD=", tools->old_pwd);
}

/**
 * @brief this function is for cd - (go back to oldpwd)
 * 
 * @param tools 
 * @return char* path of OLDPWD || NULL (OLDPWD not set)
 */
char	*mini_cd_oldpwd(t_tools *tools)
{
	char	*path;

	path = NULL;
	if (tools->old_pwd != NULL)
	{
		path = ft_strdup(tools->old_pwd);
		if (!path)
			return (malloc_error(tools->old_pwd), NULL);
		printf("%s\n", path);
	}
	else
	{
		ft_putstr_fd("Minishell: ", STDERR_FILENO);
		ft_putstr_fd("cd: OLDPWD not set\n", STDERR_FILENO);
		return (NULL);
	}
	return (path);
}

/**
 * @brief to go to home by expand_arg
 * 
 * @param tools 
 * @return (char* the path of HOME or NULL);
 */
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
