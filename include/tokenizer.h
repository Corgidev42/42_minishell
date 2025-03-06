#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

struct		s_help_token
{
	int		in_single_quote;
	int		in_double_quote;
	char	buffer[4096];
	int		buf_index;
};

struct		s_tokenizer
{
	char	**tokens;
	int		t_count;
};

/**
 * @brief Traite l'entrée utilisateur et divise en tokens.
 *
 * @param app Structure principale de l'application.
 * @param input Entrée utilisateur brute.
 * @param tokenizer Structure contenant les tokens.
 * @param hp Structure d'aide pour la gestion des tokens.
 */
void	process_tokenize(t_app *app, char *input,
			t_tokenizer *tokenizer, t_help_token *hp);

/**
 * @brief Tokenize une ligne de commande en séparant les arguments.
 *
 * @param app Structure principale de l'application.
 * @param input Chaîne contenant la ligne de commande utilisateur.
 * @return int Retourne 0 en cas de succès, 1 en cas d'erreur d'allocation.
 */
int		tokenize(t_app *app, char *input);

/**
 * @brief Ajoute un token au tokenizer.
 *
 * @param tokenizer Structure contenant les tokens.
 * @param start Début du token dans la chaîne d'entrée.
 * @param length Longueur du token.
 */
void	add_token(t_tokenizer *tokenizer, char *start, int length);

/**
 * @brief Traite une variable d'environnement dans l'entrée utilisateur.
 *
 * @param app Structure principale de l'application.
 * @param ptr Pointeur sur la position actuelle dans l'entrée utilisateur.
 * @param buffer Buffer où stocker la valeur de la variable.
 * @param buf_index Index actuel du buffer.
 */
void	handle_env_variable(t_app *app, const char **ptr,
			char *buffer, int *buf_index);

/**
 * @brief Traite un caractère de l'entrée utilisateur,
 * en tenant compte des guillemets.
 *
 * @param ptr Pointeur sur la position actuelle dans l'entrée utilisateur.
 * @param hp Structure d'aide à la gestion des tokens.
 */
void	process_character(const char **ptr, t_help_token *hp);

/**
 * @brief Initialise la structure tokenizer et l'aide à la gestion des tokens.
 *
 * @param tokenizer Structure à initialiser.
 * @param help_token Structure d'aide à initialiser.
 */
void	init_tokenizer(t_tokenizer *tokenizer, t_help_token *help_token);

/**
 * @brief Libère la mémoire allouée aux tokens du tokenizer.
 *
 * @param app Structure principale de l'application contenant le tokenizer.
 */
void	clean_tokenizer(t_app *app);

#endif
