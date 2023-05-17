/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/10 10:38:04 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/04/24 19:44:48 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


/*
Hello Naji
Project needs to be divided into the following sections:

Parsing Input:
The first step in creating a shell is to parse the input from the user.
You will need to read the input from the user and then parse it into a series of tokens that represent the command and its arguments.

Creating a Command Execution Engine:
Once the input is parsed, you will need to execute the command.
For this, you will need to create a command execution engine that can execute different types of commands, such as built-in commands like cd and exit, and external commands like ls, grep, etc.

Implementing Built-In Commands:
You will need to implement the built-in commands like cd, exit, and echo.
These commands are executed directly by the shell and do not require forking a new process.

Implementing I/O Redirection:
I/O redirection is an important feature of the shell that allows the user to redirect input and output from and to files.
You will need to implement redirection using system calls like open(), close(), and dup2().

Implementing Pipes:
Pipes allow the user to chain together multiple commands, where the output of one command is passed as input to the next command.
You will need to implement pipes using system calls like pipe() and dup2().

Implementing Job Control:
Job control is a feature that allows the user to control and manage multiple processes running in the shell.
You will need to implement job control using system calls like fork(), waitpid(), and kill().
*/

#include "minishell.h"
#include "executor.h"
#include "builtin.h"

int	main(int argc, char **argv, char **envp)
{
	char		*string;
	t_token		*tokens_head;
	t_commands	*cmds_head;
	t_tools		tools;
	char		**env_array;

	// atexit(check_leaks);
	if (argc != 1)
		return (EXIT_FAILURE);


	tokens_head = NULL;
	cmds_head = NULL;

	init_tools_env(&tools.env_list, envp);
	env_array = env_list_to_array(&tools.env_list);
	init_tools(&tools,env_array);	//keep this
	string = readline("Minishell: ");
	parse_input(string, &tokens_head);
	// print_token_list(&tokens_head, FALSE);
	printf("-----------------------\n");
	expander(&tokens_head, &tools);
	parse_cmds(&tokens_head, &cmds_head);
	print_cmds_list(&cmds_head);
	printf("-----------------------\n");
	executor(&tools,&cmds_head);
	free(string);
	free_token_list(&tokens_head);
	free_token_list(&cmds_head->redirections);
	free_cmd_list(&cmds_head);
	free_2d_arr(env_array);
	free_2d_arr(tools.envp);	//keep this 
	free_2d_arr(tools.paths);	//keep this
	free_env_list(&tools.env_list);
	(void)(argv);
	return (0);
}
