/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   buildin.h                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/09 14:07:52 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/05/09 14:07:52 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILDIN_H
# define BUILDIN_H

# include "minishell.h"

int (*execute_buildin(char *arg))(t_tools *tools, char **simple_cmd);
int	mini_pwd(t_tools *tools, char **simple_cmd);
int	mini_echo(t_tools *tools, char **simple_cmd);
int	mini_env(t_tools *tools, char **simple_cmd);
int	mini_cd(t_tools *tools, char **simple_cmd);
int	mini_echo_option(char *str);

#endif