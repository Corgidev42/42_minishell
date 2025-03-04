#ifndef APP_H
# define APP_H

# include "minishell.h"

typedef struct	s_app
{
	char		**envp;
	int			running;
	int			status;
	int			dquote;
	int			pid_current;
	t_node_ast	*first_node;
	t_tokenizer	tokenizer;
}				t_app;

int	init_app(t_app *app, char **envp);


#endif
