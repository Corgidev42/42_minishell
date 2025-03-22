/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-22 15:26:06 by ezeppa            #+#    #+#             */
/*   Updated: 2025-03-22 15:26:06 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "minishell.h"

/**
 * @brief Execute la commande echo.
 *
 * @param app Structure principale de l'application contenant l'environnement.
 * @param ast Noeud de l'AST contenant les arguments de la commande.
 * @return int Retourne toujours 0.
 */
int	exec_echo(t_app *app, t_node_ast *ast);

/**
 * @brief Change le répertoire de travail (cd) sans options.
 *
 * @param app Structure principale de l'application.
 * @param ast Noeud de l'AST contenant les arguments de la commande.
 * @return int Retourne 0 si le changement de répertoire réussit, 1 sinon.
 */
int	exec_cd(t_app *app, t_node_ast *ast);

/**
 * @brief Affiche le répertoire de travail actuel (pwd).
 *
 * @param app Structure principale de
 * l'application contenant les file descriptors.
 * @param ast Noeud de l'AST (non utilisé ici).
 * @return int Retourne toujours 0.
 */
int	exec_pwd(t_app *app, t_node_ast *ast);

/**
 * @brief Affiche les variables d'environnement.
 *
 * @param app Structure principale de l'application contenant l'environnement.
 * @param ast Noeud de l'AST (non utilisé ici).
 * @return int Retourne toujours 0.
 */
int	exec_env(t_app *app, t_node_ast *ast);

/**
 * @brief Supprime une variable d'environnement.
 *
 * @param app Structure principale de l'application contenant l'environnement.
 * @param ast Noeud de l'AST contenant la variable à supprimer.
 * @return int Retourne toujours 0.
 */
int	exec_unset(t_app *app, t_node_ast *ast);

/**
 * @brief Quitte le shell avec un code de sortie.
 *
 * @param app Structure principale de l'application.
 * @param current_node Noeud de l'AST contenant le code de sortie éventuel.
 * @return int Ne retourne jamais, car exit() est appelé.
 */
int	exec_exit(t_app *app, t_node_ast *current_node);

/**
 * @brief Vérifie si une commande est une fonction built-in.
 *
 * @param ast Noeud de l'AST contenant la commande à tester.
 * @return int Retourne 1 si la commande est une built-in, sinon 0.
 */
int	is_built_in_function(t_node_ast *ast);

/**
 * @brief Exécute une fonction built-in en fonction de la commande.
 *
 * @param app Structure principale de l'application.
 * @param ast Noeud de l'AST contenant la commande et ses arguments.
 * @return int Retourne le code de retour de la commande exécutée.
 */
int	exec_built_in(t_app *app, t_node_ast *ast);

/**
 * @brief Met à jour ou ajoute une variable d'environnement.
 *
 * @param app Structure principale de l'application contenant l'environnement.
 * @param new_entry Nouvelle variable d'environnement à ajouter ou modifier.
 * @param index Position de la variable dans envp.
 * @return int Retourne 0 en cas de succès, 1 en cas d'erreur.
 */
int	update_envp(t_app *app, char *new_entry, int index);

/**
 * @brief Ajoute ou met à jour une variable d'environnement.
 *
 * @param app Structure principale de l'application contenant l'environnement.
 * @param ast Noeud de l'AST contenant la variable à exporter.
 * @return int Retourne 0 en cas de succès, 1 si l'argument est invalide.
 */
int	exec_export(t_app *app, t_node_ast *ast);

#endif
