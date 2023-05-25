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

# include "builtin.h"
# include "minishell.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h>
# include <string.h>

# define ERROR -1
# define SUCCESS 0

char	*find_cmd_path(t_tools *tools, char **cmd);

void	executor(t_tools *tools, t_commands **cmd_head);
//check_list
char	*check_current_dir(char *cmd);

//execute
void	execute_onc_cmd(t_tools *tools, t_commands **cmd_head);
//multi cmnds
void	multi_comands(t_tools *tools, t_commands **cmd_head);
void	multi_pipex_process(t_tools *tools, t_commands **cmd_head, int *in,
			int *fd);
void	last_cmd(t_tools *tools, t_commands **cmd_head);
void	multi_v2(t_tools *tools, t_commands **cmd_head, int *fd);

//redirection
int		redirection(t_commands *cmd);
void	ft_dup2_check(int old, int new);
int		here_doc(t_token *redirection);

void	ft_pwd(void);

//error_handling
int		error_handling(char *str);
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