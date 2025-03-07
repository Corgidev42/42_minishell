#include "minishell.h"

// char *read_input(t_app *app, char **input, char *delimiter)
// {
// 	char *line;
// 	char *temp;
// 	int stdin_backup;

// 	if (app->is_heredoc == 1 && delimiter)
// 	{
// 		*input = ft_strdup("");
// 		stdin_backup = dup(STDIN_FILENO);
// 		app->is_heredoc = 1;
// 		while (1)
// 		{
// 			line = readline("heredoc>");
// 			if (!line)
// 				break ;
// 			if (ft_strcmp(line, delimiter) == 0)
// 			{
// 				free(line);
// 				break;
// 			}
// 			temp = *input;
// 			*input = ft_strjoin(temp, line);
// 			free(temp);
// 			temp = *input;
// 			*input = ft_strjoin(temp, "\n");
// 			free(temp);
// 			free(line);
// 		}
// 		app->is_heredoc = 0;
// 		dup2(stdin_backup, STDIN_FILENO);
// 		close(stdin_backup);
// 	}
// 	else
// 	{
// 		*input = readline("minishell> ");
// 		if (!*input)
// 		{
// 			app->running = 0;
// 			return NULL;
// 		}
// 		if (*input && (!app->last_input ||
// 		(ft_strcmp(*input, app->last_input) != 0)))
// 		{
// 			if (app->last_input)
// 				free(app->last_input);
// 			app->last_input = ft_strdup(*input);
// 			add_history(*input);
// 		}
// 		if (ft_strcmp(*input,"history -c") == 0)
// 		{
// 			rl_clear_history();
// 			ft_printf("Historique effacé !\n");
// 		}
// 	}
// 	return *input;
// }

void	handle_heredoc(char **input, char *delimiter)
{
	char	*line;
	char	*temp;
	int		stdin_backup;

	stdin_backup = dup(STDIN_FILENO);
	*input = ft_strdup("");
	while (1)
	{
		line = readline("heredoc>");
		if (!line || ft_strcmp(line, delimiter) == 0)
			break ;
		temp = *input;
		*input = ft_strjoin(temp, line);
		free(temp);
		free(line);
		temp = *input;
		*input = ft_strjoin(temp, "\n");
		free(temp);
	}
	free(line);
	dup2(stdin_backup, STDIN_FILENO);
	close(stdin_backup);
}

void	handle_history(t_app *app, char *input)
{
	if (!(*input))
		return ;
	if (ft_strcmp(input, "history -c") == 0)
		rl_clear_history();
	else if (!app->last_input || ft_strcmp(input, app->last_input))
	{
		free(app->last_input);
		app->last_input = ft_strdup(input);
		add_history(input);
	}
}

char	*read_input(t_app *app, char **input, char *delimiter)
{
	char	*line;

	if (app->is_heredoc && delimiter)
		handle_heredoc(input, delimiter);
	else
	{
		line = readline("minishell> ");
		if (!line)
			return (app->running = 0, NULL);
		*input = line;
		handle_history(app, *input);
	}
	return (*input);
}

int	check_entry(t_app *app, char *input)
{
	if (!input)
	{
		app->running = 0;
		return (0);
	}
	if (ft_strlen(input) == 0)
		return (0);
	return (1);
}
