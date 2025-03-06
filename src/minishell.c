#include "minishell.h"

void	minishell(t_app *app)
{
	char	*input;

	input = NULL;
	while (app->running && read_input(app, &input, NULL))
	{
		if (check_entry(app, input))
		{
			if (tokenize(app, input))
				perror("tokenize");
			app->first_node = prepare_ast(app, 0, app->tokenizer.t_count - 1);
			if (!app->first_node)
				perror("prepare ast");
			if (exec_ast(app, app->first_node))
				perror("exec ast");
			clean_tokenizer(app);
			clean_ast(app->first_node);
			app->first_node = NULL;
		}
	}
}
