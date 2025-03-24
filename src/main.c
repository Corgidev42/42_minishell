/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 15:24:43 by ezeppa            #+#    #+#             */
/*   Updated: 2025/03/24 11:07:55 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_app	*g_app(t_app *app)
{
	static t_app	*g_app;

	if (app)
		g_app = app;
	return (g_app);
}

int	main(int argc, char *argv[], char **envp)
{
	t_app	app;

	(void) argc;
	(void) argv;
	if (init_app(&app, envp))
		perror("init app");
	g_app(&app);
	minishell(&app);
	return (0);
}
