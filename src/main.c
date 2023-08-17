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

#include "builtin.h"
#include "executor.h"
#include "minishell.h"

/* Variable defined here */
int		g_exit_status = 0;



int	main(int argc, char **argv, char **envp)
{
	char		*string;
	t_token		*tokens_head;
	t_commands	*cmds_head;
	t_tools		*tools;

	// atexit(check_leaks);
	if (argc != 1)
		return (EXIT_FAILURE);
	tokens_head = NULL;
	cmds_head = NULL;
	tools = (t_tools *)malloc(sizeof(*tools));
	 if (!tools)
	{
		perror("malloc");
		return (EXIT_FAILURE);
	}
	init_tools(tools, &tokens_head, &cmds_head);
	init_tools_env(tools->env_list, envp);
	g_exit_status = 0;

	while (tools->loop)
	{
		printf("--------NEW COMMAND---------------\n");
		string = readline("Minishell: ");
		tools->og_string = ft_strdup(string);
		if (!tools->og_string)
			free_all_exit(tools);
		add_history(string);
		start_parsing(string, tools);
		printf("\n--------LEXER---------------\n");
		print_token_list(&tokens_head, FALSE);
		parse_cmds(&tokens_head, &cmds_head);
		printf("\n--------COMMANDS---------------\n");
		print_cmds_list(&cmds_head);
		printf("\n--------EXECUTION-------------\n");
		if (handle_syntax_error(&tokens_head, tools) != 1)
			executor(tools, &cmds_head);
		free_token_list(&tokens_head);
		if (cmds_head)
			free_redirection(&cmds_head);
		free_cmd_list(&cmds_head);
		free(tools->og_string);
	}
	(void)(argv);
	return (0);
}
