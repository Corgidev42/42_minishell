#include "minishell.h"


int		tokenize(t_app *app, char *input)
{
	/*
	j'initailise un int *start et un int *end avec les valeur a -1
	on aura une variable simple_quote et double_coute

	while chaque caractere jusqu'a rencontrer un espace
	tous ce qu'il y a avant token++;
	si il y a quote, je met : open_quote a TRUE, et je note son start_index (dans mon tableau d'int "start")

	j'avance tant que ja'i pas trouver l'autre quote, si $, je fais un getenv() sauf sur ' quote,
	quand je trouve l'autre quote, je met opoen_quote a FALSE, je note son end_index (dans mon tableau d'int "end"), et j'avance jusu'qa " "
	on stocke le token dans app.tokenizer + t_count++
	*/
}
void	clean_tokenizer(t_app *app)
{
	int i = 0;
	while (app->tokenizer.tokens[i])
	{
		free(app->tokenizer.tokens[i]);
		app->tokenizer.tokens[i] = NULL;
		i++;
	}
	app->tokenizer.t_count = 0;
}
