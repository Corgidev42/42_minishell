/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_ast.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-22 15:24:55 by ezeppa            #+#    #+#             */
/*   Updated: 2025-03-22 15:24:55 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node_ast	*create_ast_node(t_node_type type,
	char **args, char *filepath, char *delimiter)
{
	t_node_ast	*node;

	node = malloc(sizeof(t_node_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = args;
	node->filepath = filepath;
	node->delimiter = delimiter;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_node_ast	*create_ast_node_from_operator(t_app *app,
	int start, int end, int op_index)
{
	t_node_ast	*node;

	if (ft_strcmp(app->tokenizer.tokens[op_index], "|") == 0)
		node = create_ast_node(NODE_PIPE, NULL, NULL, NULL);
	else if (ft_strcmp(app->tokenizer.tokens[op_index], ">") == 0)
		node = create_ast_node(NODE_R_OUTPUT, NULL,
				ft_strdup(app->tokenizer.tokens[op_index + 1]), NULL);
	else if (ft_strcmp(app->tokenizer.tokens[op_index], ">>") == 0)
		node = create_ast_node(NODE_R_OUTPUT_APPEND, NULL,
				ft_strdup(app->tokenizer.tokens[op_index + 1]), NULL);
	else if (ft_strcmp(app->tokenizer.tokens[op_index], "<") == 0)
		node = create_ast_node(NODE_R_INPUT, NULL,
				ft_strdup(app->tokenizer.tokens[op_index + 1]), NULL);
	else if (ft_strcmp(app->tokenizer.tokens[op_index], "<<") == 0)
		node = create_ast_node(NODE_DELIMITER, NULL,
				NULL, ft_strdup(app->tokenizer.tokens[op_index + 1]));
	node->left = prepare_ast(app, start, op_index - 1);
	if (ft_strcmp(app->tokenizer.tokens[op_index], "|") == 0)
		node->right = prepare_ast(app, op_index + 1, end);
	else
		node->right = prepare_ast(app, op_index + 2, end);
	return (node);
}

t_node_ast	*create_ast_node_from_command(t_app *app, int start, int end)
{
	t_node_ast	*node;
	int			i;
	char		**args;

	args = malloc(sizeof(char *) * ((end - start) + 2));
	if (!args)
		return (NULL);
	i = 0;
	while (start <= end)
	{
		args[i] = ft_strdup(app->tokenizer.tokens[start]);
		start++;
		i++;
	}
	args[i] = NULL;
	node = create_ast_node(NODE_COMMAND, args, NULL, NULL);
	if (!node)
	{
		i = 0;
		while (args[i])
			free(args[i]);
		free(args);
		i++;
	}
	return (node);
}

t_node_ast	*prepare_ast(t_app *app, int start, int end)
{
	int	min_priority;
	int	op_index;
	int	i;
	int	priority;

	if (start > end)
		return (NULL);
	min_priority = 5;
	op_index = -1;
	i = start;
	while (i <= end)
	{
		priority = get_operator_priority(app->tokenizer.tokens[i]);
		if ((priority != -1) && ((priority == 1 && priority < min_priority)
				|| (priority == 2 && priority <= min_priority)
				|| (priority == 3 && priority <= min_priority)))
		{
			min_priority = priority;
			op_index = i;
		}
		i++;
	}
	if (op_index != -1)
		return (create_ast_node_from_operator(app, start, end, op_index));
	return (create_ast_node_from_command(app, start, end));
}

void	clean_ast(t_node_ast *node)
{
	int	i;

	if (!node)
		return ;
	clean_ast(node->left);
	clean_ast(node->right);
	if (node->args)
	{
		i = 0;
		while (node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	if (node->filepath)
		free(node->filepath);
	if (node->delimiter)
		free(node->delimiter);
	free(node);
}
