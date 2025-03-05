#include "minishell.h"

char *read_input(t_app *app, char **input, char *delimiter)
{
	char *line;
	char *temp;
	int stdin_backup;

	if (app->is_heredoc == 1 && delimiter)
	{
		*input = ft_strdup("");
		stdin_backup = dup(STDIN_FILENO); // Sauvegarde de STDIN
		app->is_heredoc = 1;
		while (1)
		{
			line = readline("heredoc> "); // Affiche le prompt heredoc
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
		app->is_heredoc = 0; // ✅ Désactive heredoc
		dup2(stdin_backup, STDIN_FILENO); // ✅ Restaure STDIN après interruption
		close(stdin_backup);
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
