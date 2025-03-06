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
# include "utils.h"
# include "struct.h"
# include "tokenizer.h"
# include "ast.h"
# include "libft.h"
# include "built_in.h"
# include "app.h"
# include "input.h"

void	minishell(t_app *app);

#endif
