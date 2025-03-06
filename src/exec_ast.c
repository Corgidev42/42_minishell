#include "minishell.h"

void	ast_pipe(t_app *app, t_node_ast *current_node)
{
	int	pipe_fd[2];
	app->is_in_pipe = 1;

	pipe(pipe_fd);

	// Modifier fd pour que STDOUT aille dans le pipe
	app->fd[1] = pipe_fd[1];

	// Exécuter la partie gauche (elle écrira dans le pipe)
	exec_ast(app, current_node->left);

	// Modifier fd pour que STDIN lise depuis le pipe
	app->fd[0] = pipe_fd[0];

	// Exécuter la partie droite (elle lira depuis le pipe)
	exec_ast(app, current_node->right);

	close(pipe_fd[0]);
	close(pipe_fd[1]);
	app->is_in_pipe = 0;
}

void	ast_r_input(t_app *app, t_node_ast *current_node)
{
	int fd;

	fd = open(current_node->filepath, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return;
	}

	// Modifier fd pour que STDIN lise depuis le fichier
	if (app->fd[0] == STDIN_FILENO || app->is_in_pipe)
		app->fd[0] = fd;

	// Continuer avec la commande associée
	exec_ast(app, current_node->left);

	close(fd);
}

void	ast_r_output(t_app *app, t_node_ast *current_node)
{
	int	fd = open(current_node->filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		return;
	}

	// Modifier fd pour que STDOUT écrive dans le fichier
	if (app->fd[1] == STDOUT_FILENO || app->is_in_pipe)
		app->fd[1] = fd;

	// Exécuter la commande associée
	exec_ast(app, current_node->left);

	close(fd);
}

void	ast_r_output_append(t_app *app, t_node_ast *current_node)
{
	int	fd = open(current_node->filepath, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open");
		return;
	}

	// Modifier fd pour que STDOUT ajoute dans le fichier
	if (app->fd[1] == STDOUT_FILENO || app->is_in_pipe)
		app->fd[1] = fd;

	// Exécuter la commande associée
	exec_ast(app, current_node->left);

	close(fd);
}

void	ast_delimiter(t_app *app, t_node_ast *current_node)
{
	int	pipe_fd[2];
	char *input = NULL;

	pipe(pipe_fd);

	app->is_heredoc = 1;
	read_input(app, &input, current_node->delimiter);
	if (input)
	{
		write(pipe_fd[1], input, strlen(input));
		free(input);
	}
	close(pipe_fd[1]);

	// Modifier fd pour que STDIN lise depuis le pipe
	if (app->fd[0] == STDIN_FILENO || app->is_in_pipe)
		app->fd[0] = pipe_fd[0];

	// Exécuter la commande qui doit lire depuis le heredoc
	exec_ast(app, current_node->left);

	close(pipe_fd[0]);
}

char *search_file(t_app *app, char *command)
{
	int	i;
	char	*path;
	char	**paths;
	char	*file;

	path = NULL;
	paths = NULL;
	i = 0;
	while (app->envp[i])
	{
		if (ft_strncmp(app->envp[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(app->envp[i] + 5);
			break;
		}
		i++;
	}
	if (path)
		paths = ft_split(path, ':');
	i = 0;
	while (paths && paths[i])
	{
		file = ft_strjoin(paths[i], "/");
		file = ft_strjoin(file, command);
		if (access(file, F_OK) == 0)
		{
			free(path);
			free(paths);
			return (file);
		}
		free(file);
		i++;
	}
	if (path)
		free(path);
	if (paths)
		free(paths);
	return (command);
}

void	ast_command(t_app *app, t_node_ast *current_node)
{
	int	pid;
	int	status;

	status = 0;
	if (is_built_in_function(current_node))
	{
		app->status = exec_built_in(app, current_node);
		if (app->fd[0] != STDIN_FILENO)
			close(app->fd[0]);
		if (app->fd[1] != STDOUT_FILENO)
			close(app->fd[1]);
		return;
	}
	pid = fork();
	if (pid == 0) // Processus enfant
	{
		app->pid_current = pid;
		// Appliquer les redirections avant d'exécuter la commande
		if (app->fd[0] != STDIN_FILENO)
		{
			dup2(app->fd[0], STDIN_FILENO);
			close(app->fd[0]); // Fermer après duplication
		}
		if (app->fd[1] != STDOUT_FILENO)
		{
			dup2(app->fd[1], STDOUT_FILENO);
			close(app->fd[1]); // Fermer après duplication
		}

		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		// Exécuter la commande
		execve(search_file(app, current_node->args[0]), current_node->args, app->envp);
		perror("minishell");
		exit(1);
	}
	else // Processus parent
	{
		// Le parent ne doit pas garder les fds ouverts inutilement
		if (app->fd[0] != STDIN_FILENO)
			close(app->fd[0]);
		if (app->fd[1] != STDOUT_FILENO)
			close(app->fd[1]);

		// Attendre la fin de la commande
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
		{
			// printf("Processus %d terminé par signal %d\n", pid, WTERMSIG(status));
			if (WTERMSIG(status) == SIGQUIT)
				app->status = 128 + WTERMSIG(status);
			else
				app->status = 130;
		}
		else if (WIFEXITED(status))
			app->status = WEXITSTATUS(status);
	}
}

int		exec_ast(t_app *app, t_node_ast *current_node)
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

		// Réinitialiser les fd pour éviter les conflits entre commandes successives
	app->fd[0] = STDIN_FILENO;
	app->fd[1] = STDOUT_FILENO;
	return (0);
}
