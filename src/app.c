#include "minishell.h"

static void	set_signal(t_app *app)
{
	//init tous les signaux
}

int	init_app(t_app *app, char **envp)
{
	app->envp = envp;
	app->running = 1;
	app->dquote = 1;
	app->status = 0;
	app->is_heredoc = 0;
	app->pid_current = getpid();

	set_signal(app);
	return 0;
}
