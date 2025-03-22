/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-22 15:19:25 by ezeppa            #+#    #+#             */
/*   Updated: 2025-03-22 15:19:25 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	sigint_handler(int sig, siginfo_t *info, void *context)
{
	(void)sig;
	(void)context;
	if (info->si_pid == 0)
		write(STDOUT_FILENO, "\n", 1);
	else
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sigquit_handler(int sig, siginfo_t *info, void *context)
{
	(void)sig;
	(void)context;
	(void)info;
	if (info->si_pid == 0)
		write(STDOUT_FILENO, "\n", 1);
	rl_redisplay();
}

void	set_signal(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_sigaction = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_sigaction = sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_SIGINFO;
	sigaction(SIGQUIT, &sa_quit, NULL);
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
	app->pid_current = getpid();
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
	set_signal();
	return (0);
}
