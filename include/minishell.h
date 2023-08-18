/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/10 10:38:17 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/04/10 10:38:18 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdbool.h>
# include "libft.h"
# include <limits.h>
# include <signal.h>
# include <termios.h>
# include <sys/ioctl.h>

# define DOUBLE_QUOTE 2
# define SINGLE_QUOTE 1
# define NO_QUOTATION 0
# define SYN_ERROR "Minishell: syntax error near unexpected token"
# define DOT_ERROR "Minishell: .: filename argument required\n\
.: usage: . filename [arguments]\n"

/*
	Global variable
 */
extern int	g_exit_status;

/*
	An enum to easily identify the type of the token.
*/
typedef enum s_type
{
	LITERAL = 1,
	PIPE = 2,
	IN_FILE = 3,
	HEREDOC = 4,
	REDIRECTION = 5,
	A_REDIRECTION = 6,
}	t_type;

/*
	An enum to easily identify the type of linked list.
*/
typedef enum s_lst_type
{
	TOKEN_LIST = 1,
	CMDS_LIST = 2,
}	t_lst_type;

/*
	stuct for envp_list key,value
 */
typedef struct s_env
{
	char			*key;
	char			*value;
	bool			has_value;
	struct s_env	*next;
}					t_env;

/*
	A token is part of a linked list which refers to every single
	character inputted by the user.
*/
typedef struct s_token
{
	char			*cmd;
	int				index;
	bool			valid;
	t_type			type;
	struct s_token	*next;
}				t_token;
typedef struct s_commands
{
	char				**cmds;
	char				*builtin;
	t_token				*redirections;
	char				*hd_file_name;
	struct s_commands	*next;
}				t_commands;

typedef struct s_tools
{
	char					**paths;
	char					**envp;
	t_env					**env_list;
	char					*pwd;
	char					*old_pwd;
	char					*og_string;
	t_token					**token_head;
	t_commands				**cmds_head;
	int						heredoc;
	int						hd_pid;
	bool					loop;
	bool					has_pipe;
	int						success_malloc;
}	t_tools;

/* Parsing Tokens */
int			is_whitespace(char c);
int			find_token_type(char c, char c_next);
void		start_parsing(char *string, t_tools *tools);
int			skip_whitespaces(char *string);
int			skip_space_and_return(char *string, int start);
char		*add_spaces_non_literal(char *str);

				/* Parsing Quotations */
char		*handle_quotations(char *string);
char		*handle_spaces_expansion(t_token **token_head, t_token *node);
int			check_quotations(t_token *node);
void		increment_if_not_skipped(char *new_string, int *i, int *j);
int			is_inside_quote(char *string, int pos_char);

				/* Parsing Commands */
void		parse_cmds(t_token **tokens_head, t_commands **cmd_head);
t_commands	*create_cmd(t_token *start_node, t_token *target_node,
				t_commands **cmd_head, int num_nodes);
int			is_builtin(char *string);

/* Parsing Redirection */
bool		create_redirection_list(t_commands *node_cmds,
				t_token *start_node);
bool		add_node_redirection_list(t_commands *node_cmds,
				t_token *start_node);
bool		handle_redirection(t_commands *node_cmds,
				t_token *start_node);
void		free_redirection(t_commands **cmds_head);

/* Init Tools */
char		**ft_arrdup(char **arr);
char		**find_path(char **envp);
void		add_bslash_path(char **paths);
void		init_tools(t_tools *tools, t_token **tokens_head,
				t_commands **cmds_head);
void		handler_sigint(int s);
void		handler_sigquit(int s);
void		handler_hd_sigint(int s);

				/* Linked_List Functions */
void		*last_node(void *lst, t_lst_type type);
void		add_node_back(void **lst_head, void *node, t_lst_type type);
t_token		*create_node(t_token **tokens_head, char *string, int start, int j);
t_token		*get_prev_node(t_token **tokens_head, t_token *node);
int			get_lstsize(t_token *lst);

				/* Execution of Commands */
// void	execute(t_tools *tools, t_commands **cmd_head);

				/* Expander ($ARG) */
char		*expand_arg(char *string, t_tools *tools);

				/* Printing (DEBUGGING) */
void		print_token_list(t_token **lst_head, int print_redirection);
void		print_cmds_list(t_commands **lst_head);
void		print_2d_array(char **arr);

/* Utils */
void		check_leaks(void);
void		free_token_list(t_token **lst_head);
void		free_cmd_list(t_commands **lst_head);
void		free_2d_arr(char **arr);
int			handle_syntax_error(t_token **tokens_head,
				t_tools *tools);
void		malloc_error(void *arg);
void		set_malloc_fail(char *failed_malloc, char *string_to_free, t_tools *tools);

#endif
