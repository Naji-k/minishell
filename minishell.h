/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysrondy <ysrondy@student.codam.nl>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/10 10:38:17 by ysrondy           #+#    #+#             */
/*   Updated: 2023/04/10 10:38:18 by ysrondy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include "libft/libft.h"

# define FALSE 0
# define TRUE 1

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
	// struct s_simple_cmds	*simple_cmds;
	// t_token					*lexer_list;
	// char					*pwd;
	// char					*old_pwd;
	// int						pipes;
	// int						*pid;
	// bool					heredoc;
	// bool					reset;
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
void	parse_input(char *string, t_token **tokens_head);

				/* Parsing Commands */
void	parse_cmds(t_token **tokens_head, t_commands **cmd_head);
void	create_cmd(t_token *start_node, t_token *target_node,
			t_commands **cmd_head, int num_nodes);
int		is_builtin(char *string);

				/* Init Tools */
char	**ft_arrdup(char **arr);
char	**find_path(char **envp);
void	add_bslash_path(char **paths);
void	init_tools(t_tools *tools, char **envp_arg);

				/* Linked_List Functions */
void	*last_node(void *lst, t_lst_type type);
void	add_node_back(void **lst_head, void *node, t_lst_type type);
void	create_node(t_token **tokens_head, char *string, int start, int j);

				/* Execution of Commands */
void	execute(t_tools *tools, t_commands **cmd_head);

				/* Printing (DEBUGGING) */
void	print_token_list(t_token **lst_head);
void	print_cmds_list(t_commands **lst_head);
void	print_2d_array(char **arr);

				/* Utils */
void	check_leaks(void);
void	free_token_list(t_token **lst_head);
void	free_cmd_list(t_commands **lst_head);
void	free_2d_arr(char **arr);

#endif
