/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/09 14:07:52 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/09 14:07:52 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"
# include <stdbool.h>

int		run_builtin(char *arg, t_tools *tools, char **simple_cmd);
/* PWD */
int		mini_pwd(t_tools *tools, char **simple_cmd);
/* echo */
int		mini_echo(t_tools *tools, char **simple_cmd);
int		mini_echo_option(char *str);
int		mini_echo_checker(char *str, t_tools *tools);
char	*echo_home(char *str);

/* export */
int		mini_export(t_tools *tools, char **simple_cmd);
t_env	*modify_env_value(t_env **env_list, char *simple_command,
			bool plus_equal);

/* cd */
int		mini_cd(t_tools *tools, char **simple_cmd);
void	update_pwd_env(t_tools *tools, char *tmp_opwd);
char	*cd_root_dir(char *simple_cmd);
char	*mini_cd_oldpwd(t_tools *tools);
char	*cd_home_dir(t_tools *tools);
char	*cd_sub_dir(char *simple_cmd);
/* unset */
int		mini_unset(t_tools *tools, char **simple_cmd);
/* exit */
int		mini_exit(t_tools *tools, char **simple_cmd);
void	free_all_exit(t_tools *tools);
/* env */
int		mini_env(t_tools *tools, char **simple_cmd);
//env_list_func
t_env	*env_new_node(char *env);
t_env	*env_last(t_env *node);
int		env_size(t_env *node);
void	env_add_back(t_env **list, t_env *new, int index);
int		env_del_one(t_env **list, char *key);
char	**get_paths(t_env **env_list);
char	**get_paths2(t_env **env_list);
void	init_tools_env(t_env **env_list, char **envp);
void	prinft_env(t_env **list);
char	**env_list_to_array(t_env **env_list);
void	free_env_list(t_env **env_list);
t_env	*find_env_by_key(t_env **env_list, char *key);
void	init_oldpwd(t_env **env_list);

#endif
