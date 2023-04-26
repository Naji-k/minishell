/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   executor.h                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: nakanoun <nakanoun@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/26 13:59:27 by nakanoun      #+#    #+#                 */
/*   Updated: 2023/04/26 13:59:27 by nakanoun      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "minishell.h"

char	*find_cmd_path(t_tools *tools, char **cmd);

int		execute_onc_cmd(t_tools *tools, t_commands **cmd_head);
int		executor(t_tools *tools, t_commands **cmd_head);
//check_list
char	*check_current_dir(char *cmd);

#endif