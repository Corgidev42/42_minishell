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
# include "tokenizer.h"
# include "ast.h"
# include "libft.h"
# include "built_in.h"
# include "app.h"
# include "input.h"

typedef struct s_app		t_app;
typedef enum e_node_type	t_node_type;
typedef struct s_tokenizer	t_tokenizer;
typedef struct s_node_ast	t_node_ast;

void	minishell(t_app *app);

#endif
