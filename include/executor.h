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
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>

char	*find_cmd_path(t_tools *tools, char **cmd);

void	executor(t_tools *tools, t_commands **cmd_head);
//check_list
char	*check_current_dir(char *cmd);

//execute
int		execute_onc_cmd(t_tools *tools, t_commands **cmd_head);
//multi cmnds
void	multi_comands(t_tools *tools, t_commands **cmd_head);
void	multi_pipex_process(t_tools *tools, t_commands **cmd_head, int *in);
void	last_cmd(t_tools *tools, t_commands **cmd_head);

//redirection
void	redirection(t_commands *cmd);
void	ft_dup2_check(int old, int new);

void	ft_pwd(void);

//builtins
int		bn_echo(t_commands **cmd_head);
void	bn_pwd(void);
#endif

/*  TEST CASES
ls -la | cat | cat | git | grep add 
ls -la | cat > 1 | echo hii > 2 | echo hello > 3 | git | grep git > 4

ls -la >out | echo hii  >2 | echo hello  >3 | git > 4			**BUG
ls -la > out | echo hii  > 2 | echo hello  > 3 | git > 4	**BUG
 */