/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 08:36:12 by ysrondy           #+#    #+#             */
/*   Updated: 2023/04/21 08:36:24 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/*
	Looks for "PATH" inside env and ft_splits them based on ":".
*/
char	**find_path(char **envp)
{
	int		i;
	char	**path_arr;

	i = 0;
	while (ft_strncmp(envp[i], "PATH", 4) != 0)
		i++;
	path_arr = ft_split(envp[i], ':');
	if (!path_arr)
		return (NULL);
	ft_strlcpy(path_arr[0], ft_strchr(path_arr[0], '/'),
		ft_strlen(path_arr[0]) - 4);
	add_bslash_path(path_arr);
	return (path_arr);
}
