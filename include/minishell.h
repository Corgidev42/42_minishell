#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct	s_app		t_app;
typedef enum	e_node_type	t_node_type;
typedef struct	s_tokenizer	t_tokenizer;
typedef struct	s_node_ast	t_node_ast;



# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "tokenizer.h"
# include "ast.h"
# include "app.h"
# include "input.h"

void	minishell(t_app *app);


#endif
