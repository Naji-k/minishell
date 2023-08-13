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
int	g_exit_status = 0;

void	handle_syntax_error(t_token **tokens_head, t_commands **cmds_head)
{
	t_token		*token;
	t_commands	*cmds;

	token = *tokens_head;
	cmds = *cmds_head;
	g_exit_status = 258;
	if (token && token->next && token->next->next && token->next->next->next && cmds == NULL)
		printf("Minishell: syntax error near unexpected token `%c%c'\n", token->next->next->cmd[0], token->next->next->next->cmd[0]);
	else if (token && token->next && token->next->next && cmds == NULL)
		printf("Minishell: syntax error near unexpected token `%c'\n", token->cmd[0]);
	else if (token && token->next && cmds == NULL)
	{
		if (token->type == PIPE && token->next->type == PIPE)
			printf("Minishell: syntax error near unexpected token `||'\n");
		else
			printf("Minishell: syntax error near unexpected token `newline'\n");
	}
	else if (token && cmds == NULL)
	{
		if (token->type == PIPE)
			printf("Minishell: syntax error near unexpected token `|'\n");
		else
			printf("Minishell: syntax error near unexpected token `newline'\n");
	}
	else
	{
		// need to change this
		g_exit_status = 0;
		return ;
	}
}
void	print_history(t_tools *tools)
{
	t_token	*head_history;

	head_history = tools->history;
	while (head_history)
	{
		printf("   %d  %s\n", head_history->index, head_history->cmd);
		head_history = head_history->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char		*string;
	t_token		*tokens_head;
	t_commands	*cmds_head;
	t_tools		*tools;

	//atexit(check_leaks);
	if (argc != 1)
		return (EXIT_FAILURE);
	tokens_head = NULL;
	cmds_head = NULL;
	tools = (t_tools *)malloc(sizeof(*tools));
	init_tools_env(&tools->env_list, envp);
	init_tools(tools, &tokens_head, &cmds_head); //keep this
	g_exit_status = 0;

	while (tools->loop)
	{
		printf("\n--------NEW COMMAND---------------\n");
		string = readline("Minishell: ");
		tools->og_string = ft_strdup(string);
		if (!tools->og_string)
			exit(EXIT_FAILURE);
		add_history(string);
		parse_input(string, &tokens_head, tools);
		printf("\n--------LEXER---------------\n");
		print_token_list(&tokens_head, FALSE);
		parse_cmds(&tokens_head, &cmds_head);
		printf("\n--------COMMANDS---------------\n");
		print_cmds_list(&cmds_head);
		printf("\n--------EXECUTION-------------\n");
		handle_syntax_error(&tokens_head, &cmds_head);
		executor(tools, &cmds_head);
		free_token_list(&tokens_head);
		if (cmds_head)
			free_redirection(&cmds_head);
		free_cmd_list(&cmds_head);
		free(tools->og_string);
	}
	// free_2d_arr(tools->envp);	//keep this
	// free_2d_arr(tools->paths);	//keep this
	// free_token_list(&tools->history);
	//do not have to free them when submit the project
	/* 	free_env_list(&tools->env_list);
	free(tools->pwd);
	free(tools->old_pwd);
	free(tools); */
	(void)(argv);
	return (0);
}
