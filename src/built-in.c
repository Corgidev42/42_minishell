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

// Fonction cd with no options
int	exec_cd(t_app *app, t_node_ast *ast)
{
	if (ast->args[1] == NULL)
	{
		if (chdir(getenv("HOME")) == -1)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			return (1);
		}
	}
	else if (chdir(ast->args[1]) == -1)
	{
		ft_putstr_fd("cd: Not a directory: ", STDERR_FILENO);
		ft_putstr_fd(ast->args[1], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

// Fonction pwd with no options
int	exec_pwd(t_app *app, t_node_ast *ast)
{
	char	cwd[128];

	ft_putstr_fd(getcwd(cwd, 128), STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}
// Fonction export with no options
int	exec_export(t_app *app, t_node_ast *ast)
{
	int	i;
	int	len_to_equal;

	if (ft_strchr(ast->args[1], '=') == NULL)
	{
		ft_putstr_fd("export: not a valid identifier\n", STDERR_FILENO);
		return (1);
	}
	len_to_equal = ft_strchr(ast->args[1], '=') - ast->args[1];
	i = 0;
	while (app->envp[i] && ft_strncmp(ast->args[1], app->envp[i], len_to_equal) != 0)
		i++;
	if (app->envp[i] == NULL)
	{
		app->envp = ft_realloc(app->envp, sizeof(char *) * (i + 2));
		if (app->envp == NULL)
			return (1);
		app->envp[i] = ft_strdup(ast->args[1]);
		if (app->envp[i] == NULL)
			return (1);
		app->envp[i + 1] = NULL;
	}
	else
	{
		free(app->envp[i]);
		app->envp[i] = ft_strdup(ast->args[1]);
		if (app->envp[i] == NULL)
			return (1);
	}
	return (0);
}

int exec_unset(t_app *app, t_node_ast *ast)
{
	if (!ast->args[1]) // Si aucun argument, ne rien faire
		return (0);

	int i = 0;
	int len_to_equal = ft_strlen(ast->args[1]);

	while (app->envp[i])
	{
		if (ft_strncmp(ast->args[1], app->envp[i], len_to_equal) == 0 && app->envp[i][len_to_equal] == '=')
			break;
		i++;
	}

	if (app->envp[i]) // Si trouvé
	{
		free(app->envp[i]); // Supprime la variable
		while (app->envp[i]) // Décale les suivantes
		{
			app->envp[i] = app->envp[i + 1];
			i++;
		}
	}

	return (0);
}
// Fonction env with no options
int	exec_env(t_app *app, t_node_ast *ast)
{
	int	i;

	i = 0;
	while (app->envp[i])
	{
		ft_putstr_fd(app->envp[i], STDOUT_FILENO);
		ft_putstr_fd("\n", STDOUT_FILENO);
		i++;
	}
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
