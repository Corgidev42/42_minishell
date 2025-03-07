#ifndef AST_BUILDER_H
# define AST_BUILDER_H

# include "minishell.h"

/**
 * @brief Crée un nœud d'AST avec les informations données.
 *
 * @param type Type du nœud (commande, opérateur, etc.).
 * @param args Arguments de la commande (si applicable).
 * @param filepath Chemin du fichier pour la redirection (si applicable).
 * @param delimiter Délimiteur pour heredoc (si applicable).
 * @return t_node_ast* Retourne un pointeur vers le nœud créé.
 */
t_node_ast	*create_ast_node(t_node_type type, char **args,
				char *filepath, char *delimiter);

/**
 * @brief Crée un nœud d'AST basé sur un opérateur trouvé dans la commande.
 *
 * @param app Structure principale de l'application.
 * @param start Index de début de la commande.
 * @param end Index de fin de la commande.
 * @param op_index Index de l'opérateur dans la commande.
 * @return t_node_ast* Retourne un pointeur vers le nœud créé.
 */
t_node_ast	*create_ast_node_from_operator(t_app *app, int start,
				int end, int op_index);

/**
 * @brief Crée un nœud d'AST représentant une commande.
 *
 * @param app Structure principale de l'application.
 * @param start Index de début de la commande.
 * @param end Index de fin de la commande.
 * @return t_node_ast* Retourne un pointeur vers le nœud créé.
 */
t_node_ast	*create_ast_node_from_command(t_app *app, int start, int end);

/**
 * @brief Prépare l'AST en analysant les tokens et en organisant les nœuds.
 *
 * @param app Structure principale de l'application.
 * @param start Index de début de l'analyse.
 * @param end Index de fin de l'analyse.
 * @return t_node_ast* Retourne la racine de l'AST construit.
 */
t_node_ast	*prepare_ast(t_app *app, int start, int end);

/**
 * @brief Libère la mémoire allouée à l'AST.
 *
 * @param node Nœud racine de l'AST à libérer.
 */
void		clean_ast(t_node_ast *node);

#endif
