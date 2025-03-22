/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-22 15:24:43 by ezeppa            #+#    #+#             */
/*   Updated: 2025-03-22 15:24:43 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char *argv[], char **envp)
{
	t_app	app;

	(void) argc;
	(void) argv;
	if (init_app(&app, envp))
		perror("init app");
	minishell(&app);
	return (0);
}
