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
# include <sys/stat.h>

# define ERROR -1
# define SUCCESS 0

char	*find_cmd_path(t_tools *tools, char *cmd);

void	executor(t_tools *tools, t_commands **cmd_head);
//check_list
char	*check_current_dir(char *cmd);

//execute
void	one_cmd_handler(t_tools *tools, t_commands **cmd_head);
void	execute_onc_cmd(t_tools *tools, t_commands **cmd_head);
int		close_pipes(int *fd, int old_fd);
void	execve_cmd(t_tools *tools, t_commands **cmd_head);
//multi cmnds
void	multi_commands_handler(t_tools *tools, t_commands **cmd_head);
int		multi_pipex_process(t_tools *tools, t_commands **cmd_head, int old_fd,
			int *fd);
pid_t	last_cmd(t_tools *tools, t_commands **last_cmd, int old_fd);
void	multi_v2(t_tools *tools, t_commands **cmd_head, int *fd);
void	wait_last_pid(pid_t last_pid);

//redirection
int		redirection(t_commands *cmd);
void	ft_dup2_check(int old, int new);
int		create_heredoc(t_token *redirection, t_commands *cmd, t_tools *tools);
int		is_heredoc(t_commands **cmd, t_tools *tools);

//error_handling
int		error_file_handling(char *str);
int		e_cmd_not_found(char *s_cmd);
//builtins
int		bn_echo(t_commands **cmd_head);
void	bn_pwd(void);
#endif

/*  TEST CASES
ls -la | cat | cat | git | grep add
ls -la | cat > 1 | echo hii > 2 | echo hello > 3 | git | grep git > 4

ls -la >out | echo hii  >2 | echo hello  >3 | git > 4			**BUG
ls -la > out | echo hii  > 2 | echo hello  > 3 | git > 4	**BUG
<< 'EOF' cat\n$USER\nab$USER\EOF
cat << EOF > file.txt\n$USER\nab$USER\EOF
<< EOF1 << EOF2 << EOF3
 */
