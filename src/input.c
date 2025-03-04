#include "minishell.h"

char *read_input(t_app *app, char **input)
{
	*input = readline("minishell> "); // Affiche le prompt
	if (!input)
	{ // Si Ctrl+D (EOF), quitter
		printf("\nAu revoir !\n");
	}
	if (*input) {
		add_history(*input);
	}
	printf("Commande entrée : %s\n", *input);
	return *input;
}
int	check_entry(t_app *app, char *input)
{
	return 0;
}
