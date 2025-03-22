/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-22 15:26:15 by ezeppa            #+#    #+#             */
/*   Updated: 2025-03-22 15:26:15 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "minishell.h"

/**
 * @brief Gère l'entrée de l'utilisateur en mode heredoc.
 *
 * @param input Pointeur vers la chaîne où sera stockée l'entrée.
 * @param delimiter Délimiteur indiquant la fin de l'entrée heredoc.
 */
void	handle_heredoc(char **input, char *delimiter);

/**
 * @brief Gère l'historique des commandes entrées par l'utilisateur.
 *
 * @param app Structure principale de l'application.
 * @param input Chaîne contenant l'entrée utilisateur actuelle.
 */
void	handle_history(t_app *app, char *input);

/**
 * @brief Lit l'entrée utilisateur et gère les modes interactif et heredoc.
 *
 * @param app Structure principale de l'application.
 * @param input Pointeur vers la chaîne contenant l'entrée utilisateur.
 * @param delimiter Délimiteur heredoc (si applicable).
 * @return char* Retourne l'entrée utilisateur ou NULL en cas de fin du shell.
 */
char	*read_input(t_app *app, char **input, char *delimiter);

/**
 * @brief Vérifie si l'entrée utilisateur est valide.
 *
 * @param app Structure principale de l'application.
 * @param input Chaîne contenant l'entrée utilisateur.
 * @return int Retourne 1 si l'entrée est valide, 0 sinon.
 */
int		check_entry(t_app *app, char *input);

#endif
