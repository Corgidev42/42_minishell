/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-22 15:25:21 by ezeppa            #+#    #+#             */
/*   Updated: 2025-03-22 15:25:21 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*minishell_getenv(t_app *app, char *str)
{
	int	i;

	i = 0;
	if (ft_strcmp(str, "?") == 0)
	{
		return (ft_itoa(app->status));
	}
	while (app->envp[i])
	{
		if (ft_strncmp(app->envp[i], str, ft_strlen(str)) == 0)
			if (app->envp[i][ft_strlen(str)] == '=')
				return (app->envp[i] + ft_strlen(str) + 1);
		i++;
	}
	return (NULL);
}

int	get_operator_priority(char *token)
{
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
