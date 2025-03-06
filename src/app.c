#include "minishell.h"
#include <signal.h>

t_app *g_app;

// Gestionnaire pour SIGINT (CTRL + C)
void sigint_handler(int sig, siginfo_t *info, void *context)
{
	(void)sig;
	(void)context;
	if (g_app->is_heredoc)
	{
		g_app->is_heredoc = 0;
		close(STDIN_FILENO);   // ✅ Ferme STDIN pour interrompre readline()
	}
	else
	{
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
}

// Gestionnaire pour SIGQUIT (CTRL + \)
void sigquit_handler(int sig)
{
	(void)sig;
	rl_redisplay();
}

// Configuration des signaux pour le shell interactif
static void set_signal(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	// Configuration de SIGINT (CTRL + C)
	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_SIGINFO; // Evite les interruptions de readline()
	sigaction(SIGINT, &sa_int, NULL);

	// Configuration de SIGQUIT (CTRL + \)
	sa_quit.sa_handler = sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

char	*minishell_getenv(t_app *app, char *str)
{
	int i = 0;
	if (ft_strcmp(str, "?") == 0)
	{
		return (ft_itoa(app->status));
	}
	while (app->envp[i])
	{
		if (ft_strncmp(app->envp[i], str, ft_strlen(str)) == 0)
			if (app->envp[i][ft_strlen(str)] == '=')
				return (app->envp[i] + ft_strlen(str) + 1);
		i++;
	}
	return (NULL);
}

int	init_app(t_app *app, char **envp)
{
	g_app = app;
	app->running = 1;
	app->dquote = 1;
	app->status = 0;
	app->is_heredoc = 0;
	app->is_in_pipe = 0;
	app->fd[0] = STDIN_FILENO;
	app->fd[1] = STDOUT_FILENO;
	app->pid_current = getpid();
	app->last_input = NULL;
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

	set_signal();
	return 0;
}
