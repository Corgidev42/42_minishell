#ifndef AST_EXEC_H
# define AST_EXEC_H

# include "minishell.h"

enum			e_node_type
{
	NODE_COMMAND,
	NODE_DELIMITER,
	NODE_R_INPUT,
	NODE_R_OUTPUT,
	NODE_R_OUTPUT_APPEND,
	NODE_PIPE
};

struct			s_node_ast
{
	t_node_type			type;
	char				**args;
	char				*filepath;
	char				*delimiter;
	struct s_node_ast	*left;
	struct s_node_ast	*right;
};

/**
 * @brief Exécute un nœud de type pipe en gérant les entrées/sorties.
 *
 * @param app Structure principale de l'application.
 * @param current_node Nœud de l'AST contenant l'opérateur pipe.
 */
void	ast_pipe(t_app *app, t_node_ast *current_node);

/**
 * @brief Gère la redirection d'entrée (fichier en lecture "<").
 *
 * @param app Structure principale de l'application.
 * @param current_node Nœud de l'AST contenant le fichier à lire.
 */
void	ast_r_input(t_app *app, t_node_ast *current_node);

/**
 * @brief Gère la redirection de sortie (écriture dans un fichier ">" ).
 *
 * @param app Structure principale de l'application.
 * @param current_node Nœud de l'AST contenant le fichier à écrire.
 */
void	ast_r_output(t_app *app, t_node_ast *current_node);

/**
 * @brief Gère la redirection de sortie en mode ajout (">>").
 *
 * @param app Structure principale de l'application.
 * @param current_node Nœud de l'AST contenant le fichier à écrire.
 */
void	ast_r_output_append(t_app *app, t_node_ast *current_node);

/**
 * @brief Gère l'opérateur heredoc ("<<").
 *
 * @param app Structure principale de l'application.
 * @param current_node Nœud de l'AST contenant le délimiteur.
 */
void	ast_delimiter(t_app *app, t_node_ast *current_node);

/**
 * @brief Exécute une commande built-in et gère les descripteurs de fichiers.
 *
 * @param app Structure principale de l'application.
 * @param current_node Nœud de l'AST contenant la commande.
 */
void	exec_builtin_command(t_app *app, t_node_ast *current_node);

/**
 * @brief Gère la sortie d'un processus enfant.
 *
 * @param app Structure principale de l'application.
 * @param status Statut de sortie du processus enfant.
 */
void	handle_child_exit(t_app *app, int status);

/**
 * @brief Prépare un processus enfant en redirigeant les entrées/sorties
 * et en réinitialisant les signaux.
 *
 * @param app Structure principale de l'application.
 */
void	prepare_child_process(t_app *app);

/**
 * @brief Exécute une commande normale (externe ou built-in)
 * en créant un processus enfant si nécessaire.
 *
 * @param app Structure principale de l'application.
 * @param current_node Nœud de l'AST contenant la commande à exécuter.
 */
void	ast_command(t_app *app, t_node_ast *current_node);

/**
 * @brief Récupère les chemins d'exécution
 * depuis la variable d'environnement PATH.
 *
 * @param app Structure principale de l'application.
 * @return char** Tableau de chemins d'exécution ou NULL en cas d'erreur.
 */
char	**get_paths_from_env(t_app *app);

/**
 * @brief Recherche un fichier exécutable dans les chemins définis dans PATH.
 *
 * @param app Structure principale de l'application.
 * @param command Commande à rechercher.
 * @return char* Chemin absolu du fichier si trouvé,
 * sinon la commande inchangée.
 */
char	*search_file(t_app *app, char *command);

/**
 * @brief Exécute un nœud de l'AST en fonction de son type.
 *
 * @param app Structure principale de l'application.
 * @param current_node Nœud de l'AST à exécuter.
 * @return int Retourne 0 en cas de succès, 1 en cas d'erreur.
 */
int		exec_ast(t_app *app, t_node_ast *current_node);

#endif
