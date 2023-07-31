/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init_tools.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/21 08:36:12 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/04/21 08:36:24 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

/*
	Adding a backslash after each path to make it easier for me to find
	the executables later on.
*/
void	add_bslash_path(char **paths)
{
	char	*tmp;
	int		i;

	i = 0;
	while (paths[i] != NULL)
	{
		if (ft_strncmp(&paths[i][ft_strlen(paths[i]) - 1], "/", 1) != 0)
		{
			tmp = ft_strjoin(paths[i], "/");
			free(paths[i]);
			paths[i] = tmp;
		}
		i++;
	}
}


	// Looks for "PATH" inside env and ft_splits them based on ":".

 char	**find_path(char **envp)
{
	int		i;
	char	**path_arr;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH", 5) == 0)
		{
			path_arr = ft_split(envp[i], ':');
			if (!path_arr)
				return (NULL);
			ft_strlcpy(path_arr[0], ft_strchr(path_arr[0], '/'),
					ft_strlen(path_arr[0]) - 4);
			add_bslash_path(path_arr);
			return (path_arr);
		}
		i++;
	}
	return (NULL);
}

char	**get_paths2(t_env **env_list)
{
	char	**path_arr;
	t_env	*path;

	path_arr = NULL;
	path = find_env_by_key(env_list, "PATH");
	if (!path)
		return (NULL);
	path_arr = ft_split(path->value, ':');
	add_bslash_path(path_arr);
	return (path_arr);
}

void	init_tools(t_tools *tools, t_token **tokens_head, t_commands **cmds_head)
{
	// tools->envp = ft_arrdup(envp_arg);
	// tools->paths = find_path(tools->envp);
	tools->old_pwd = NULL;
	tools->pwd = getcwd(NULL, sizeof(PATH_MAX));
	tools->loop = true;
	tools->has_pipe = false;
	tools->heredoc = 0;
	tools->history = NULL;
	tools->token_head = tokens_head;
	tools->cmds_head = cmds_head;
}
