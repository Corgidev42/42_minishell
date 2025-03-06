#include "minishell.h"

int	exec_echo(t_app *app, t_node_ast *ast)
{
	int	i;
	int	n_flag;

	i = 1;
	n_flag = 0;
	if (ast->args[i] && ft_strcmp(ast->args[i], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	while (ast->args[i])
	{
		ft_putstr_fd(ast->args[i], STDOUT_FILENO);
		if (ast->args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

int	exec_cd(t_app *app, t_node_ast *ast)
{
	return (0);
}

int	exec_pwd(t_app *app, t_node_ast *ast)
{
	return (0);
}

int	exec_export(t_app *app, t_node_ast *ast)
{
	return (0);
}

int	exec_unset(t_app *app, t_node_ast *ast)
{
	return (0);
}

int	exec_env(t_app *app, t_node_ast *ast)
{
	return (0);
}

int	exec_exit(t_app *app, t_node_ast *current_node)
{
	int exit_code = app->status; // Par défaut, utiliser le dernier code de retour
	if (current_node->args[1])  // Si un argument est fourni, l'utiliser comme code de sortie
		exit_code = ft_atoi(current_node->args[1]);

	clean_ast(current_node);  // Libérer l'AST avant de quitter
	printf("Exiting minishell with code %d\n", exit_code);
	exit(exit_code);
	return (0);
}

int	is_built_in_function(t_node_ast *ast)
{
	if (ft_strcmp(ast->args[0], "echo") == 0)
		return (1);
	if (ft_strcmp(ast->args[0], "cd") == 0)
		return (1);
	if (ft_strcmp(ast->args[0], "pwd") == 0)
		return (1);
	if (ft_strcmp(ast->args[0], "export") == 0)
		return (1);
	if (ft_strcmp(ast->args[0], "unset") == 0)
		return (1);
	if (ft_strcmp(ast->args[0], "env") == 0)
		return (1);
	if (ft_strcmp(ast->args[0], "exit") == 0)
		return (1);
	return (0);
}

int	exec_built_in(t_app *app, t_node_ast *ast)
{
	if (ft_strcmp(ast->args[0], "echo") == 0)
		return (exec_echo(app, ast));
	if (ft_strcmp(ast->args[0], "cd") == 0)
		return (exec_cd(app, ast));
	if (ft_strcmp(ast->args[0], "pwd") == 0)
		return (exec_pwd(app, ast));
	if (ft_strcmp(ast->args[0], "export") == 0)
		return (exec_export(app, ast));
	if (ft_strcmp(ast->args[0], "unset") == 0)
		return (exec_unset(app, ast));
	if (ft_strcmp(ast->args[0], "env") == 0)
		return (exec_env(app, ast));
	if (ft_strcmp(ast->args[0], "exit") == 0)
		return (exec_exit(app, ast));
	return (0);
}
