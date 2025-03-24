/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 15:25:31 by ezeppa            #+#    #+#             */
/*   Updated: 2025/03/24 11:10:04 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APP_H
# define APP_H

# include "minishell.h"

struct	s_app
{
	pid_t			current_pid;
	char			**envp;
	int				running;
	int				is_heredoc;
	int				is_in_pipe;
	int				status;
	int				dquote;
	int				fd[2];
	char			*last_input;
	t_node_ast		*first_node;
	t_tokenizer		tokenizer;
};

/**
 * @brief Gestionnaire pour SIGINT (CTRL + C).
 *
 * @param sig Numéro du signal reçu.
 */
void	sigint_handler(int sig);

/**
 * @brief Gestionnaire pour SIGQUIT (CTRL + \).
 *
 * @param sig Numéro du signal reçu.
 */
void	sigquit_handler(int sig);

/**
 * @brief Configure les signaux pour le shell interactif.
 */
void	set_signal(void);

/**
 * @brief Initialise les valeurs par défaut de la structure t_app.
 *
 * @param app Pointeur sur la structure principale de l'application.
 */
void	init_brut(t_app *app);

/**
 * @brief Initialise la structure de l'application et copie l'environnement.
 *
 * @param app Pointeur sur la structure principale de l'application.
 * @param envp Tableau des variables d'environnement du système.
 * @return int Retourne 0 en cas de succès, 1 en cas d'erreur d'allocation.
 */
int		init_app(t_app *app, char **envp);

#endif
