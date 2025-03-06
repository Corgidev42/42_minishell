#include "minishell.h"
#include <signal.h>

t_app *g_app;

// Gestionnaire pour SIGINT (CTRL + C)
void sigint_handler(int sig)
{
	(void)sig;
	if (g_app->is_heredoc)
	{
		g_app->is_heredoc = 0;
		close(STDIN_FILENO);   // ✅ Ferme STDIN pour interrompre readline()
		write(1, "\nHeredoc annulé\n", 16);
	}
	else
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

// Gestionnaire pour SIGQUIT (CTRL + \)
void sigquit_handler(int sig)
{
	(void)sig;
}

// Configuration des signaux pour le shell interactif
static void set_signal(t_app *app)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	// Configuration de SIGINT (CTRL + C)
	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART; // Evite les interruptions de readline()
	sigaction(SIGINT, &sa_int, NULL);

	// Configuration de SIGQUIT (CTRL + \)
	sa_quit.sa_handler = sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
}


int	init_app(t_app *app, char **envp)
{
	g_app = app;
	app->running = 1;
	app->dquote = 1;
	app->status = 0;
	app->is_heredoc = 0;
	app->fd[0] = STDIN_FILENO;
	app->fd[1] = STDOUT_FILENO;
	app->pid_current = getpid();
	app->first_node = NULL;
	app->tokenizer.t_count = 0;

	// Copie de l'environnement
	int i = 0;
	while (envp[i])
		i++;
	app->envp = malloc(sizeof(char *) * (i + 1));
	if (!app->envp)
		return 1;
	i = 0;
	while (envp[i])
	{
		app->envp[i] = ft_strdup(envp[i]);
		if (!app->envp[i])
			return 1;
		i++;
	}
	app->envp[i] = NULL;

	set_signal(app);
	return 0;
}
