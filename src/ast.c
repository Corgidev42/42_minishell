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
		if (priority != -1 && priority <= min_priority)
		{
			min_priority = priority;
			op_index = i;
		}
		i++;
	}
	// Si un opérateur a été trouvé, créer le nœud correspondant
	if (op_index != -1)
	{
		t_node_type type;
		if (ft_strcmp(app->tokenizer.tokens[op_index], "|") == 0)
		{
			type = NODE_PIPE;
			t_node_ast *node = create_ast_node(NODE_PIPE, NULL, NULL, NULL);
			node->left = prepare_ast(app, start, op_index - 1);
			node->right = prepare_ast(app, op_index + 1, end);
			return node;
		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], ">") == 0)
		{
			type = NODE_R_OUTPUT;
			t_node_ast *node = create_ast_node(NODE_R_OUTPUT, NULL, app->tokenizer.tokens[op_index + 1], NULL);
			node->left = prepare_ast(app, start, op_index - 1);
			node->right = prepare_ast(app, op_index + 2, end);
			return node;
		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], ">>") == 0)
		{
			type = NODE_R_OUTPUT_APPEND;
			t_node_ast *node = create_ast_node(NODE_R_OUTPUT_APPEND, NULL, app->tokenizer.tokens[op_index + 1], NULL);
			node->left = prepare_ast(app, start, op_index - 1);
			node->right = prepare_ast(app, op_index + 2, end);
			return node;
		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], "<") == 0)
		{
			type = NODE_R_INPUT;
			t_node_ast *node = create_ast_node(NODE_R_INPUT, NULL, app->tokenizer.tokens[op_index + 1], NULL);
			node->left = prepare_ast(app, start, op_index - 1);
			node->right = prepare_ast(app, op_index + 2, end);
			return node;
		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], "<<") == 0)
		{
			type = NODE_DELIMITER;
			t_node_ast *node = create_ast_node(NODE_DELIMITER, NULL, NULL, app->tokenizer.tokens[op_index + 1]);
			node->left = prepare_ast(app, start, op_index - 1);
			node->right = prepare_ast(app, op_index + 2, end);
			return node;
		}
		else
			return (NULL); // cas où un token de type command n'aurais pas l'index -1
	}
	// sinon on creer un noeud de type command
	char **args = NULL;
	args = malloc(sizeof(char*) * (end - start) + 2);
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
        printf("REDIR IN: %s\n", ast->args ? ast->args[0] : "(null)");
    else if (ast->type == NODE_R_OUTPUT)
        printf("REDIR OUT: %s\n", ast->args ? ast->args[0] : "(null)");
    else if (ast->type == NODE_R_OUTPUT_APPEND)
        printf("APPEND: %s\n", ast->args ? ast->args[0] : "(null)");
    else if (ast->type == NODE_DELIMITER)
        printf("HEREDOC: %s\n", ast->args ? ast->args[0] : "(null)");

	if (ast->left)
    	print_ast(ast->left, level + 1);
	if (ast->right)
    	print_ast(ast->right, level + 1);
}

void	ast_pipe(t_app *app, t_node_ast *current_node)
{
	int	pipe_fd[2];

	pipe(pipe_fd);
	if (fork() == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		exec_ast(app, current_node->left);
		exit(0);
	}
	if (fork() == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		exec_ast(app, current_node->right);
		exit(0);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait(NULL);
	wait(NULL);
}

void	ast_r_input(t_app *app, t_node_ast *current_node)
{
	int fd = open(current_node->filepath, O_RDONLY);

	if (fd == -1)
	{
		perror("open");
		return;
	}
	if (fork() == 0)
	{
		dup2(fd, STDIN_FILENO);
		close(fd);
		exec_ast(app, current_node->left);
		exit(0);
	}
	close(fd);
	wait(NULL);
}

void	ast_r_output(t_app *app, t_node_ast *current_node)
{
	int	fd;

	fd = open(current_node->filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		return;
	}
	if (fork() == 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
		exec_ast(app, current_node->left);
		exit(0);
	}
	close(fd);
	wait(NULL);

	// dup2(fd, STDOUT_FILENO);
	// close(fd);
	// exec_ast(app, current_node->left);
}

void	ast_r_output_append(t_app *app, t_node_ast *current_node)
{
	int	fd;

	fd = open(current_node->filepath, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("open");
		return;
	}
	if (fork() == 0)
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
		exec_ast(app, current_node->left);
		exit(0);
	}
	close(fd);
	wait(NULL);
}

void	ast_delimiter(t_app *app, t_node_ast *current_node)
{
	int	pipe_fd[2];
	char *input;

	pipe(pipe_fd);
	input = NULL;
	app->is_heredoc = 1;
	read_input(app, &input, current_node->delimiter);
	if (input)
	{
		write(pipe_fd[1], input, strlen(input));
		free(input);
	}
	close(pipe_fd[1]);
	if (fork() == 0)
	{
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
		exec_ast(app, current_node->left);
		exit(0);
	}
	close(pipe_fd[0]);
	wait(NULL);
}

char *search_file(t_app *app, char *command)
{
	int	i;
	char	*path;
	char	**paths;
	char	*file;

	// TODO : Lorsque l'on aura nos built in passer par les notre d'abord
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
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
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
	free(path);
	free(paths);
	return (command);
}

void	ast_command(t_app *app, t_node_ast *current_node)
{
	int	pid;
	int	status;

	pipe(app->fd);
	pid = fork();
	if (pid == 0)
	{
		dup2(app->fd[0] , STDIN_FILENO);
		dup2(app->fd[1] , STDOUT_FILENO);
		close(app->fd[0]);
		close(app->fd[1]);
		execve(search_file(app, current_node->args[0]), current_node->args, app->envp);
		perror("minishell");
		exit(1);
	}
	else
	{
		close(app->fd[0]);
		close(app->fd[1]);
		waitpid(pid, &status, 0);
		app->status = WEXITSTATUS(status);
	}
}

int		exec_ast(t_app *app, t_node_ast *current_node)
{
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
	return (0);
}

void	clean_ast(t_app *app)
{

}
