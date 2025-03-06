#include "minishell.h"

char	**get_paths_from_env(t_app *app)
{
	int		i;
	char	*path;
	char	**paths;

	path = NULL;
	i = 0;
	while (app->envp[i])
	{
		if (ft_strncmp(app->envp[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(app->envp[i] + 5);
			break ;
		}
		i++;
	}
	paths = ft_split(path, ':');
	if (path)
		free(path);
	return (paths);
}

char	*search_file(t_app *app, char *command)
{
	int		i;
	char	**paths;
	char	*file;

	paths = get_paths_from_env(app);
	i = 0;
	while (paths && paths[i])
	{
		file = ft_strjoin(paths[i], "/");
		file = ft_strjoin(file, command);
		if (access(file, F_OK) == 0)
		{
			free(paths);
			return (file);
		}
		free(file);
		i++;
	}
	if (paths)
		free(paths);
	return (command);
}

int	exec_ast(t_app *app, t_node_ast *current_node)
{
	if (!current_node)
		return (1);
	if (current_node->type == NODE_PIPE)
		ast_pipe(app, current_node);
	else if (current_node->type == NODE_R_INPUT)
		ast_r_input(app, current_node);
	else if (current_node->type == NODE_R_OUTPUT)
		ast_r_output(app, current_node);
	else if (current_node->type == NODE_R_OUTPUT_APPEND)
		ast_r_output_append(app, current_node);
	else if (current_node->type == NODE_DELIMITER)
		ast_delimiter(app, current_node);
	else if (current_node->type == NODE_COMMAND)
		ast_command(app, current_node);
	app->fd[0] = STDIN_FILENO;
	app->fd[1] = STDOUT_FILENO;
	return (0);
}
