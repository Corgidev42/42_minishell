#ifndef INPUT_H
# define INPUT_H

# include "minishell.h"

char	*read_input(t_app *app, char **input, char *delimiter);
int		check_entry(t_app *app, char *input);

#endif
