#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "libft.h"
# include "struct.h"
# include "utils.h"
# include "tokenizer.h"
# include "ast_builder.h"
# include "ast_exec.h"
# include "libft.h"
# include "built_in.h"
# include "app.h"
# include "input.h"

/**
 * @brief Boucle principale du shell minishell.
 *
 * @param app Structure principale de l'application.
 */
void	minishell(t_app *app);

#endif
