#include "minishell.h"

void	minishell(t_app *app)
{
	char *input;

	input = NULL;
	while (app->running && read_input(app, &input, NULL)) //si heredoc, on affiche "heredoc>"
	{
		if (check_entry(app, input))	//on regarde l'entre du prompt et si y'a pas de ctrl d
		{
			if (tokenize(app, input))	//remplie struct tokenizer en decoupant tous l'input correctement en prennant comptes les quotes
				perror("tokenize");
			app->first_node = prepare_ast(app, 0, app->tokenizer.t_count - 1);
			if (!app->first_node)	//prepare les branches de l'ast
				perror("prepare ast");
			print_ast(app->first_node, 0);
			if (exec_ast(app, app->first_node)) //execute l'ast dans le bonne ordre
				perror("exec ast");
			clean_tokenizer(app);
			clean_ast(app);
		}
	}
}
