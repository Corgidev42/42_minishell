#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

/**
 * @brief Duplique une chaîne avec une longueur maximale.
 *
 * @param s Chaîne source à dupliquer.
 * @param n Nombre maximum de caractères à copier.
 * @return char* Une nouvelle chaîne allouée, ou NULL en cas d'échec.
 */
char	*ft_strndup(const char *s, size_t n);

/**
 * @brief Copie une chaîne dans une autre avec une longueur maximale.
 *
 * @param dst Destination de la copie.
 * @param src Source de la copie.
 * @param len Nombre maximum de caractères à copier.
 * @return char* Retourne dst après la copie.
 */
char	*ft_strncpy(char *dst, const char *src, size_t len);

/**
 * @brief Récupère la valeur d'une variable d'environnement dans Minishell.
 *
 * @param app Structure principale de l'application contenant l'environnement.
 * @param str Nom de la variable d'environnement.
 * @return char* Valeur de la variable ou NULL si elle n'existe pas.
 */
char	*minishell_getenv(t_app *app, char *str);
int		get_operator_priority(char *token);

#endif
