#include "minishell.h"

int	exec_exit(t_app *app, t_node_ast *current_node)
{
	int	exit_code;

	exit_code = app->status;
	if (current_node->args[1])
		exit_code = ft_atoi(current_node->args[1]);
	clean_ast(current_node);
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

int	update_envp(t_app *app, char *new_entry, int index)
{
	if (app->envp[index] == NULL)
	{
		app->envp = ft_realloc(app->envp, sizeof(char *) * (index + 2));
		if (!app->envp)
			return (1);
		app->envp[index + 1] = NULL;
	}
	else
	{
		free(app->envp[index]);
	}
	app->envp[index] = ft_strdup(new_entry);
	if (app->envp[index] == NULL)
		return (1);
	return (0);
}

int	exec_export(t_app *app, t_node_ast *ast)
{
	int	i;
	int	len_to_equal;

	if (!ft_strchr(ast->args[1], '='))
		return \
		(ft_putstr_fd("export: not a valid identifier\n", STDERR_FILENO), 1);
	len_to_equal = ft_strchr(ast->args[1], '=') - ast->args[1];
	i = 0;
	while (app->envp[i]
		&& (ft_strncmp(ast->args[1], app->envp[i], len_to_equal) != 0
			|| app->envp[i][len_to_equal] != '='))
		i++;
	return (update_envp(app, ast->args[1], i));
}

// int	exec_export(t_app *app, t_node_ast *ast)
// {
// 	int	i;
// 	int	len_to_equal;

// 	if (ft_strchr(ast->args[1], '=') == NULL)
// 	{
// 		ft_putstr_fd("export: not a valid identifier\n", STDERR_FILENO);
// 		return (1);
// 	}
// 	len_to_equal = ft_strchr(ast->args[1], '=') - ast->args[1];
// 	i = 0;
// 	while (app->envp[i]
// 		&& (ft_strncmp(ast->args[1], app->envp[i], len_to_equal) != 0
// 			|| app->envp[i][len_to_equal] != '='))
// 		i++;
// 	if (app->envp[i] == NULL)
// 	{
// 		app->envp = ft_realloc(app->envp, sizeof(char *) * (i + 2));
// 		if (app->envp == NULL)
// 			return (1);
// 		app->envp[i] = ft_strdup(ast->args[1]);
// 		if (app->envp[i] == NULL)
// 			return (1);
// 		app->envp[i + 1] = NULL;
// 	}
// 	else
// 	{
// 		free(app->envp[i]);
// 		app->envp[i] = ft_strdup(ast->args[1]);
// 		if (app->envp[i] == NULL)
// 			return (1);
// 	}
// 	return (0);
// }
