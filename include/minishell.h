#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include "app.h"
# include "ast.h"
# include "tokenizer.h"

void	minishell(t_app *app);


#endif
