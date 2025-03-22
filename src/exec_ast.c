/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-22 15:23:26 by ezeppa            #+#    #+#             */
/*   Updated: 2025-03-22 15:23:26 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ast_pipe(t_app *app, t_node_ast *current_node)
{
	int	pipe_fd[2];

	app->is_in_pipe = 1;
	pipe(pipe_fd);
	app->fd[1] = pipe_fd[1];
	exec_ast(app, current_node->left);
	app->fd[0] = pipe_fd[0];
	exec_ast(app, current_node->right);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	app->is_in_pipe = 0;
}

void	ast_r_input(t_app *app, t_node_ast *current_node)
{
	int	fd;

	fd = open(current_node->filepath, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return ;
	}
	if (app->fd[0] == STDIN_FILENO || app->is_in_pipe)
		app->fd[0] = fd;
	exec_ast(app, current_node->left);
	close(fd);
}

void	ast_r_output(t_app *app, t_node_ast *current_node)
{
	int	fd;

	fd = open(current_node->filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		return ;
	}
	if (app->fd[1] == STDOUT_FILENO || app->is_in_pipe)
		app->fd[1] = fd;
	exec_ast(app, current_node->left);
	close(fd);
}

void	ast_r_output_append(t_app *app, t_node_ast *current_node)
{
	int	fd;

	fd = open(current_node->filepath, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open");
		return ;
	}
	if (app->fd[1] == STDOUT_FILENO || app->is_in_pipe)
		app->fd[1] = fd;
	exec_ast(app, current_node->left);
	close(fd);
}

void	ast_delimiter(t_app *app, t_node_ast *current_node)
{
	int		pipe_fd[2];
	char	*input;

	input = NULL;
	pipe(pipe_fd);
	app->is_heredoc = 1;
	read_input(app, &input, current_node->delimiter);
	if (input)
	{
		write(pipe_fd[1], input, strlen(input));
		free(input);
	}
	close(pipe_fd[1]);
	if (app->fd[0] == STDIN_FILENO || app->is_in_pipe)
		app->fd[0] = pipe_fd[0];
	exec_ast(app, current_node->left);
	close(pipe_fd[0]);
}
