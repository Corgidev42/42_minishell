#ifndef APP_H
# define APP_H

# include "minishell.h"

struct	s_app
{
	char		**envp;
	int			running;
	int			is_heredoc;
	int			status;
	int			dquote;
	int			pid_current;
	t_node_ast	*first_node;
	t_tokenizer	tokenizer;
};

int	init_app(t_app *app, char **envp);


#endif
