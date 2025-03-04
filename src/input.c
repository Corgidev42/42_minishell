#include "minishell.h"

char *read_input(t_app *app, char **input, char *delimiter)
{
	char *line;
	char *temp;
	if (!*input) { // Vérifie si readline() retourne NULL
		ft_printf("\nAu revoir !\n");
		return NULL;
	}
	if (app->is_heredoc == 1 && delimiter)
	{
		*input = ft_strdup("");
		while (1)
		{
			app->is_heredoc = 1;
			line = readline("heredoc> "); // Affiche le prompt heredoc
			app->is_heredoc = 0;
			// ft_printf("Ligne lue : %s\n", line);
			if (!line) // CTRL + D détecté
			{
				ft_printf("\nwarning: here-document delimited by end-of-file\n");
				break; // Quitte la boucle heredoc proprement
			}
			if (ft_strcmp(line, delimiter) == 0) // Si on tape le délimiteur, on arrête
			{
				free(line);
				break;
			}
			temp = *input;
			*input = ft_strjoin(temp, line);
			free(temp);
			temp = *input;
			*input = ft_strjoin(temp, "\n");
			free(temp);
			free(line);
		}
	}
	else
		{
			*input = readline("minishell> "); // Affiche le prompt
			if (!*input) // CTRL + D détecté
			{
				ft_printf("\nAu revoir !\n");
				app->running = 0; // Quitter le shell proprement
				return NULL;
			}
			if (*input)
				add_history(*input);
			if (ft_strcmp(*input,"history -c") == 0)
			{
				rl_clear_history();
				ft_printf("Historique effacé !\n");
			}
			ft_printf("Commande entrée : %s\n", *input);
		}
	ft_printf("Contenu de input : %s\n", *input);
	return *input;
}

int	check_entry(t_app *app, char *input)
{
	if (!input)
	{
		app->running = 0;
		return 0;
	}
	return 1;
}
