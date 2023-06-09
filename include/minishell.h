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
# include <unistd.h>
# include <stdbool.h>
# include "libft.h"
# include <limits.h>

# define FALSE 0
# define TRUE 1
# define ADD_QUOTATION 1
# define ADD_QUOTATION_BEGIN 2
# define ADD_QUOTATION_END 3
# define NO_QUOTATION 0
# define SKIP 666

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
	t_type			type;
	struct s_token	*next;
}				t_token;

typedef struct s_tools
{
	// char					*args;
	char					**paths;
	char					**envp;
	t_env					*env_list;
	// struct s_simple_cmds	*simple_cmds;
	t_token					*history;
	char					*pwd;
	char					*old_pwd;
	char					*og_string;
	// int						pipes;
	// int						*pid;
	int						heredoc;
	bool					loop;
	bool					has_pipe;
}	t_tools;


typedef struct s_commands
{
	char				**cmds;
	char				*builtin; // needs to become function call
	t_token				*redirections;
	char				*hd_file_name; // not sure
	struct s_commands	*next;
}				t_commands;


				/* Parsing Tokens */
int		is_whitespace(char c);
int		find_token_type(char c, char c_next);
void	parse_input(char *string, t_token **tokens_head, t_tools *tools);

				/* Parsing Dollar Expander */
void	handle_arg_then_dollar(char *new_string, char *string, int i, int j);
int		handle_dbl_dollars(char *new_string, char *string, int i, int j);
char	*sep_dollars(char *string);

				/* Parsing Quotations */
char	*handle_quotations(char *string);
char	*substring(t_token *node, int i);
char	*add_single_quote(char *string, int location);

				/* Parsing Commands */
void	parse_cmds(t_token **tokens_head, t_commands **cmd_head);
void	create_cmd(t_token *start_node, t_token *target_node,
			t_commands **cmd_head, int num_nodes);
int		is_builtin(char *string);

				/* Init Tools */
char	**ft_arrdup(char **arr);
char	**find_path(char **envp);
void	add_bslash_path(char **paths);
void	init_tools(t_tools *tools);

				/* Linked_List Functions */
void	*last_node(void *lst, t_lst_type type);
void	add_node_back(void **lst_head, void *node, t_lst_type type);
t_token	*create_node(t_token **tokens_head, char *string, int start, int j);

				/* Execution of Commands */
// void	execute(t_tools *tools, t_commands **cmd_head);

				/* Expander ($ARG) */
void	expander(t_token **lst_tokens, t_tools *tools);
char	*expand_arg(char *string, t_tools *tools);
void	expansion_into_token_list(t_token **lst_tokens, t_token *node,
			char *expanded_arg, int single_quote);
t_token	*handle_expansion(t_token **lst_tokens, t_token *node,
			t_tools *tools, int single_quote);


				/* Printing (DEBUGGING) */
void	print_token_list(t_token **lst_head, int print_redirection);
void	print_cmds_list(t_commands **lst_head);
void	print_2d_array(char **arr);

				/* Utils */
void	check_leaks(void);
void	free_token_list(t_token **lst_head);
void	free_cmd_list(t_commands **lst_head);
void	free_2d_arr(char **arr);

#endif
