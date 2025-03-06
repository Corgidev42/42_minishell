#include "minishell.h"

int get_operator_priority(char *token) {
	if (ft_strcmp(token, "<<") == 0)
		return (3);
	if (ft_strcmp(token, "<") == 0)
		return (3);
	if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">>") == 0)
		return (2);
	if (ft_strcmp(token, "|") == 0)
		return (1);
	return (-1);
}

t_node_ast	*create_ast_node(t_node_type type, char **args, char *filepath, char *delimiter)
{
	t_node_ast	*node = malloc(sizeof(t_node_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = args;
	node->filepath = filepath;
	node->delimiter = delimiter;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_node_ast	*prepare_ast(t_app *app, int start, int end)
{
	if (start > end)
		return (NULL);

	// Chercher l'opérateur principal en respectant les priorités
	int	min_priority = 5;  // max prio
	int	op_index = -1;
	int	i = start;
	while (i <= end)
	{
		int priority = get_operator_priority(app->tokenizer.tokens[i]);
		if ((priority != -1)
			&& ((priority == 1 && priority < min_priority)
			|| (priority == 2 && priority <= min_priority)
			|| (priority == 3 && priority <= min_priority)))
		{
			min_priority = priority;
			op_index = i;
		}
		i++;
	}
	// Si un opérateur a été trouvé, créer le nœud correspondant
	if (op_index != -1)
	{
		if (ft_strcmp(app->tokenizer.tokens[op_index], "|") == 0)
		{
			t_node_ast *node = create_ast_node(NODE_PIPE, NULL, NULL, NULL);
			node->left = prepare_ast(app, start, op_index - 1);
			node->right = prepare_ast(app, op_index + 1, end);
			return node;
		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], ">") == 0)
		{
			t_node_ast *node = create_ast_node(NODE_R_OUTPUT, NULL, ft_strdup(app->tokenizer.tokens[op_index + 1]), NULL);
			node->left = prepare_ast(app, start, op_index - 1);
			node->right = prepare_ast(app, op_index + 2, end);
			return node;
		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], ">>") == 0)
		{
			t_node_ast *node = create_ast_node(NODE_R_OUTPUT_APPEND, NULL, ft_strdup(app->tokenizer.tokens[op_index + 1]), NULL);
			node->left = prepare_ast(app, start, op_index - 1);
			node->right = prepare_ast(app, op_index + 2, end);
			return node;
		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], "<") == 0)
		{
			t_node_ast *node = create_ast_node(NODE_R_INPUT, NULL, ft_strdup(app->tokenizer.tokens[op_index + 1]), NULL);
			node->left = prepare_ast(app, start, op_index - 1);
			node->right = prepare_ast(app, op_index + 2, end);
			return node;
		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], "<<") == 0)
		{
			t_node_ast *node = create_ast_node(NODE_DELIMITER, NULL, NULL, ft_strdup(app->tokenizer.tokens[op_index + 1]));
			node->left = prepare_ast(app, start, op_index - 1);
			node->right = prepare_ast(app, op_index + 2, end);
			return node;
		}
		else
			return (NULL); // cas où un token de type command n'aurais pas l'index -1
	}
	// sinon on creer un noeud de type command
	char **args;
	args = malloc(sizeof(char*) * ((end - start) + 2));
	if (!args)
		return (NULL);
	i = 0;
	while (start <= end)
	{
		args[i] = ft_strdup(app->tokenizer.tokens[start]);
		start++;
		i++;
	}
	args[i] = NULL;
	t_node_ast *node = create_ast_node(NODE_COMMAND, args , NULL, NULL);
	return node;
}

/**
 * Fonction d'affichage pour le debug de l'AST
 * ls -l | wc -l
 */
void print_ast(t_node_ast *ast, int level)
{
    if (!ast) return;
    for (int i = 0; i < level; i++)
        printf("  ");
    if (ast->type == NODE_COMMAND)
	{
		printf("CMD: %s\n", ast->args[0]);
	}
    else if (ast->type == NODE_PIPE)
        printf("PIPE\n");
    else if (ast->type == NODE_R_INPUT)
        printf("REDIR IN: %s\n", ast->filepath);
    else if (ast->type == NODE_R_OUTPUT)
        printf("REDIR OUT: %s\n", ast->filepath);
    else if (ast->type == NODE_R_OUTPUT_APPEND)
        printf("APPEND: %s\n", ast->filepath);
    else if (ast->type == NODE_DELIMITER)
        printf("HEREDOC: %s\n", ast->delimiter);
	else
	{}

	if (ast->left)
    	print_ast(ast->left, level + 1);
	if (ast->right)
    	print_ast(ast->right, level + 1);
}

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

void clean_ast(t_node_ast *node)
{
	if (!node)
		return;
	// Libérer récursivement les sous-arbres gauche et droit
	clean_ast(node->left);
	clean_ast(node->right);
	// Libérer les arguments si c'est une commande
	if (node->args)
	{
		for (int i = 0; node->args[i]; i++)
			free(node->args[i]);
		free(node->args);
	}
	// Libérer le nom de fichier si c'est une redirection
	if (node->filepath)
		free(node->filepath);
	// Libérer le délimiteur si c'est un heredoc
	if (node->delimiter)
		free(node->delimiter);
	// Libérer le nœud lui-même
	free(node);
}
