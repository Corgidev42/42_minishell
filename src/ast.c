#include "minishell.h"

int get_operator_priority(char *token) {
	if (ft_strcmp(token, "<<") == 0)
		return (1);
	if (ft_strcmp(token, "<") == 0)
		return (2);
	if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">>") == 0)
		return (3);
	if (ft_strcmp(token, "|") == 0)
		return (4);
	return (-1);
}

t_node_ast	*create_ast_node(t_node_type type, char **args, t_node_ast *left, t_node_ast *right)
{
	t_node_ast	*node = malloc(sizeof(t_node_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = args;
	node->left = left;
	node->right = right;
	return (node);
}

int		prepare_ast(t_app *app, int start, int end)
{
	printf("token : %s\n", app->tokenizer.tokens[start]);
	printf("1.1\n");
	if (start > end)
		return (NULL);

	// Chercher l'opérateur principal en respectant les priorités
	printf("1.2\n");
	int	min_priority = 5;  // max prio
	int	op_index = -1;
	int	i = start;
	printf("start : %d\n", start);
	printf("end : %d\n", end);
	printf("i : %d\n", i);
	while (i < end)
	{
		int priority = get_operator_priority(app->tokenizer.tokens[i]);
		if (priority != -1 && priority <= min_priority)
		{
			min_priority = priority;
			op_index = i;
		}
		i++;
	}
	printf("1.3\n");

	// Si un opérateur a été trouvé, créer le nœud correspondant
	if (op_index != -1)
	{
		t_node_type type;
		printf("1.3.0\n");
		if (ft_strcmp(app->tokenizer.tokens[op_index], "|") == 0)
		{
			type = NODE_PIPE;
			printf("1.3.1\n");
		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], ">") == 0)
		{
			type = NODE_R_OUTPUT;
			printf("1.3.2\n");

		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], ">>") == 0)
		{
			type = NODE_R_OUTPUT_APPEND;
			printf("1.3.3\n");

		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], "<") == 0)
		{
			type = NODE_R_INPUT;
			printf("1.3.4\n");
		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], "<<") == 0)
		{
			type = NODE_DELIMITER;
			printf("1.3.5\n");

		}
		else
		{
			printf("1.3.6\n");
			return (NULL); // cas où un token de type command n'aurais pas l'index -1
		}
		printf("1.3.7\n");
		char **args = NULL;
		if (type == NODE_R_OUTPUT || type == NODE_R_OUTPUT_APPEND || type == NODE_R_INPUT || type == NODE_DELIMITER)
		{
			if (op_index + 1 <= end)
				args = &app->tokenizer.tokens[op_index + 1];
		}

		int	next_start = op_index + 1;
		if (args)
			next_start += 1;
		return (create_ast_node(type,
								args,
								prepare_ast(app, start, op_index - 1),
								prepare_ast(app, next_start, end)));
	}
	printf("1.4\n");
	// sinon on creer un noeud de type command
	return	create_ast_node(NODE_COMMAND, &app->tokenizer.tokens[start], NULL, NULL);
}

/**
 * Fonction d'affichage pour le debug de l'AST
 */
void print_ast(t_node_ast *ast, int level) {
    if (!ast) return;

    for (int i = 0; i < level; i++)
        printf("  ");

    if (ast->type == NODE_COMMAND)
        printf("CMD: %s\n", ast->args[0]);
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

    print_ast(ast->left, level + 1);
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
		write(pipe_fd[1], "\n", 1);
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

void	ast_command(t_app *app, t_node_ast *current_node)
{
	int	pid;
	int	status;

	pid = fork();
	if (pid == 0)
	{
		execve(current_node->args[0], current_node->args, app->envp);
		perror("execve");
		exit(1);
	}
	else
	{
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
}

void	clean_ast(t_app *app)
{

}
