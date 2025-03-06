#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

struct		s_help_token
{
	int		in_single_quote;
	int		in_double_quote;
	char	buffer[4096];
	int		buf_index;
};

struct		s_tokenizer
{
	char	**tokens;
	int		t_count;
};

void	add_token(t_tokenizer *tokenizer, char *start, int length);
void	handle_env_variable(t_app *app, const char **ptr, char *buffer, int *buf_index);
void	process_character(const char **ptr, t_help_token *hp);
void	init_tokenizer(t_tokenizer *tokenizer, t_help_token *help_token);
void	clean_tokenizer(t_app *app);
int		tokenize(t_app *app, char *input);

#endif
