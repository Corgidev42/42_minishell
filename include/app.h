#ifndef APP_H
# define APP_H

# include "minishell.h"

extern struct s_app	*g_app;

struct	s_app
{
	char			**envp;
	int				running;
	int				is_heredoc;
	int				status;
	int				dquote;
	int				fd[2];
	int				pid_current;
	t_node_ast		*first_node;
	t_tokenizer		tokenizer;
};

int		init_app(t_app *app, char **envp);
void	sigint_handler(int sig);
void	sigquit_handler(int sig);

#endif
