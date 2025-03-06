#ifndef UTILS_H
# define UTILS_H

# include "minishell.h"

char	*ft_strndup(const char *s, size_t n);
char	*ft_strncpy(char *dst, const char *src, size_t len);
char	*minishell_getenv(t_app *app, char *str);

#endif