#include "minishell.h"

int		prepare_ast(t_app *app, int start, int end)
{
	/*
	1er bloc :
	je parocours tous mes tokens et defini mes priorite en fonction des operateur et leurs index, si il n'en a pas, c'est -1

	2e bloc :
	je prend l'index postif
	je creer mon node avec son status correspodnant
	si type R_X, prendre l'argument de droite comme filepath
	si type DELIMITER, prendre l'argument de droite comme delimiteur
	et je creer 2 branche, gauche qui aura pour start : start, et end : index_token - 1
	droite qui aura pour start : index_token + 1(+2 si delimiteur ou redirect) et end : end;

	3e bloc :
	je creer un node de type commande
	il prend le premier, et il met de start a index comme argument
	*/
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
