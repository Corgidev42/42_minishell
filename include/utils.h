/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 15:27:01 by ezeppa            #+#    #+#             */
/*   Updated: 2025/03/24 11:07:15 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

/**
 * @brief Récupère la structure principale de l'application.
 * 
 * @param app Nouvelle structure principale de l'application.
 * @return t_app* Structure principale de l'application.
 */
t_app	*g_app(t_app *app);

/**
 * @brief Récupère la valeur d'une variable d'environnement dans Minishell.
 *
 * @param app Structure principale de l'application contenant l'environnement.
 * @param str Nom de la variable d'environnement.
 * @return char* Valeur de la variable ou NULL si elle n'existe pas.
 */
char	*minishell_getenv(t_app *app, char *str);

/**
 * @brief Récupère la priorité d'un opérateur.
 *
 * @param token Opérateur à évaluer.
 * @return int Priorité de l'opérateur.
 */
int		get_operator_priority(char *token);

#endif
