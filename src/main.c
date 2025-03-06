#include "minishell.h"

int	main(int argc, char *argv[], char **envp)
{
	t_app	app;

	(void) argc;
	(void) argv;
	if (init_app(&app, envp))
		perror("init app");
	minishell(&app);
	return (0);
}
