/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: ysrondy <ysrondy@student.codam.nl>           +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/04/25 09:00:46 by ysrondy       #+#    #+#                 */
/*   Updated: 2023/04/25 09:01:07 by ysrondy       ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "builtin.h"
#include "executor.h"

char	*search_value_expansion(char *string, int *len,
			t_tools *tools, int found_equal_sign)
{
	t_env	*env_list;
	char	*expanded_arg;

	env_list = *tools->env_list;
	while (env_list)
	{
		if (ft_strncmp((string + 1), env_list->key, *len) == 0
			&& ft_strlen(env_list->key) >= *len && env_list->key[*len] == '=')
		{
			if (found_equal_sign == true)
			{
				expanded_arg = ftp_strjoin(env_list->value,
						&string[(*len) + 1]);
			}
			else
				expanded_arg = ft_strdup(env_list->value);
			set_malloc_fail(expanded_arg, string, tools);
			return (expanded_arg);
		}
		env_list = env_list->next;
	}
	return (NULL);
}

char	*expand_heredoc(t_token *node, char *line, t_tools *tools)
{
	char	*expanded_string;
	char	*final_string;
	int		i;

	i = 0;
	final_string = calloc((ft_strlen(line) + 1), sizeof(char));
	if (!final_string)
		return (malloc_error(line), NULL);
	expanded_string = NULL;
	while (line[i] != '\0')
	{
		if (should_expand(line, tools, node, i) == true)
		{
			expanded_string = get_expanded_arg(line, tools, &i, node);
			if (!expanded_string || tools->success_malloc == false)
				return (free(line), free(final_string), NULL);
			final_string = ftp_strjoin(final_string, expanded_string);
			if (!final_string)
				return (malloc_error(line), free(expanded_string), NULL);
			free(expanded_string);
			if (line[i] == '~' && i == 0)
				i++;
		}
		else
		{
			final_string = ft_str_add_char(final_string, line[i]);
			if (!final_string)
				return (free(line), NULL);
			i++;
		}
	}
	free(line);
	return (final_string);
}

char	*copy_expanded_string(char *line, int *i,
	char *str_to_be_expanded, int *j)
{
	while ((is_whitespace(line[*i]) == false && line[*i] != '\0')
		&& (line[*i] != '"' && line[*i] != '\''))
	{
		str_to_be_expanded[*j] = line[*i];
		if (line[*i] == '~')
		{
			(*j) += 1;
			break ;
		}
		(*i) += 1;
		(*j) += 1;
		if (line[*i] == '$' || line[*i] == '-'
			|| line[*i] == '~' || line[*i] == '+') // maybe check all special chars, also ^
		{
			if (line[*i] == '$' && line[*i - 1] == '$')
			{
				str_to_be_expanded[*j] = line[*i];
				(*i) += 1;
				(*j) += 1;
			}
			break ;
		}
	}
	str_to_be_expanded[*j] = '\0';
	return (str_to_be_expanded);
}

char	*get_expanded_arg(char *line, t_tools *tools, int *i, t_token *node)
{
	char	*str_to_be_expanded;
	char	*expanded_string;
	int		j;

	j = 0;
	tools->success_malloc = true;
	str_to_be_expanded = malloc(sizeof(char) * (ft_strlen(line) + 1));
	if (!str_to_be_expanded)
		return (malloc_error(NULL), NULL);
	str_to_be_expanded = copy_expanded_string(line, i, str_to_be_expanded, &j);
	expanded_string = expand_arg(str_to_be_expanded, tools);
	if (!expanded_string)
	{
		if (tools->success_malloc == false)
			return (NULL);
		node->valid = false;
		if (get_prev_node(tools->token_head, node)->type != LITERAL
			&& get_prev_node(tools->token_head, node)->type != PIPE)
			return (str_to_be_expanded);
		str_to_be_expanded[0] = '\0';
		return (str_to_be_expanded);
	}
	printf("Expanded: %s to %s\n", str_to_be_expanded, expanded_string);
	free(str_to_be_expanded);
	return (expanded_string);
}
