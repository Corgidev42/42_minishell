/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-22 15:23:16 by ezeppa            #+#    #+#             */
/*   Updated: 2025-03-22 15:23:16 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		ft_putstr_fd(ast->args[i], app->fd[1]);
		if (ast->args[i + 1])
			ft_putstr_fd(" ", app->fd[1]);
		i++;
	}
	if (!n_flag)
		ft_putstr_fd("\n", app->fd[1]);
	return (0);
}

// Fonction cd with no options
int	exec_cd(t_app *app, t_node_ast *ast)
{
	(void)app;
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

	(void)ast;
	ft_putstr_fd(getcwd(cwd, 128), app->fd[1]);
	ft_putstr_fd("\n", app->fd[1]);
	return (0);
}

int	exec_env(t_app *app, t_node_ast *ast)
{
	int	i;

	(void)ast;
	i = 0;
	while (app->envp[i])
	{
		ft_putstr_fd(app->envp[i], app->fd[1]);
		ft_putstr_fd("\n", app->fd[1]);
		i++;
	}
	return (0);
}

int	exec_unset(t_app *app, t_node_ast *ast)
{
	int	i;
	int	len_to_equal;

	if (!ast->args[1])
		return (0);
	i = 0;
	len_to_equal = ft_strlen(ast->args[1]);
	while (app->envp[i])
	{
		if (ft_strncmp(ast->args[1], app->envp[i], len_to_equal) == 0
			&& app->envp[i][len_to_equal] == '=')
			break ;
		i++;
	}
	if (app->envp[i])
	{
		free(app->envp[i]);
		while (app->envp[i])
		{
			app->envp[i] = app->envp[i + 1];
			i++;
		}
	}
	return (0);
}
