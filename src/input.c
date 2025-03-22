/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-22 15:24:03 by ezeppa            #+#    #+#             */
/*   Updated: 2025-03-22 15:24:03 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
