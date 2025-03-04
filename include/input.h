#ifndef INPUT_H
#define INPUT_H

# include "minishell.h"

char *read_input(t_app *app, char **input);
int	check_entry(t_app *app, char *input);

#endif
