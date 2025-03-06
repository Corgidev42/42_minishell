#ifndef APP_H
# define APP_H

# include "minishell.h"

struct	s_app
{
	char			**envp;
	int				running;
	int				is_heredoc;
	int				is_in_pipe;
	int				status;
	int				dquote;
	int				fd[2];
	int				pid_current;
	char			*last_input;
	t_node_ast		*first_node;
	t_tokenizer		tokenizer;
};

int		init_app(t_app *app, char **envp);
// void	sigint_handler(int sig);
void	sigquit_handler(int sig);
char	*minishell_getenv(t_app *app, char *str);

#endif
