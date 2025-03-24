/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 15:19:25 by ezeppa            #+#    #+#             */
/*   Updated: 2025/03/24 11:13:45 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	if (g_app(NULL)->current_pid != 0)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_app(NULL)->status = 130;
	}
}

void	sigquit_handler(int sig)
{
	(void)sig;
	if (g_app(NULL)->current_pid == 0)
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	rl_redisplay();
}

void	set_signal(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigquit_handler);
}

void	init_brut(t_app *app)
{
	app->running = 1;
	app->is_heredoc = 0;
	app->is_in_pipe = 0;
	app->status = 0;
	app->dquote = 1;
	app->fd[0] = STDIN_FILENO;
	app->fd[1] = STDOUT_FILENO;
	app->last_input = NULL;
	app->first_node = NULL;
	app->tokenizer.tokens = NULL;
	app->tokenizer.t_count = 0;
}

int	init_app(t_app *app, char **envp)
{
	int	i;

	init_brut(app);
	i = 0;
	while (envp[i])
		i++;
	app->envp = malloc(sizeof(char *) * (i + 1));
	if (!app->envp)
		return (1);
	i = 0;
	while (envp[i])
	{
		app->envp[i] = ft_strdup(envp[i]);
		if (!app->envp[i])
			return (1);
		i++;
	}
	app->envp[i] = NULL;
	app->current_pid = getpid();
	set_signal();
	return (0);
}
