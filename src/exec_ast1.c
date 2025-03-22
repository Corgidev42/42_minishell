/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-22 15:23:36 by ezeppa            #+#    #+#             */
/*   Updated: 2025-03-22 15:23:36 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_builtin_command(t_app *app, t_node_ast *current_node)
{
	app->status = exec_built_in(app, current_node);
	if (app->fd[0] != STDIN_FILENO)
		close(app->fd[0]);
	if (app->fd[1] != STDOUT_FILENO)
		close(app->fd[1]);
}

void	handle_child_exit(t_app *app, int status)
{
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGQUIT)
			app->status = 128 + WTERMSIG(status);
		else
			app->status = 130;
	}
	else if (WIFEXITED(status))
		app->status = WEXITSTATUS(status);
}

void	prepare_child_process(t_app *app)
{
	if (app->fd[0] != STDIN_FILENO)
	{
		dup2(app->fd[0], STDIN_FILENO);
		close(app->fd[0]);
	}
	if (app->fd[1] != STDOUT_FILENO)
	{
		dup2(app->fd[1], STDOUT_FILENO);
		close(app->fd[1]);
	}
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	ast_command(t_app *app, t_node_ast *current_node)
{
	int	pid;
	int	status;

	status = 0;
	if (is_built_in_function(current_node))
	{
		exec_builtin_command(app, current_node);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		app->pid_current = pid;
		prepare_child_process(app);
		execve(search_file(app, current_node->args[0]),
			current_node->args, app->envp);
		perror("minishell");
		exit(1);
	}
	if (app->fd[0] != STDIN_FILENO)
		close(app->fd[0]);
	if (app->fd[1] != STDOUT_FILENO)
		close(app->fd[1]);
	waitpid(pid, &status, 0);
	handle_child_exit(app, status);
}
