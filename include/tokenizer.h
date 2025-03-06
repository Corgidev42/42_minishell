#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "minishell.h"

struct		s_tokenizer
{
	char	**tokens;
	int		t_count;
};

int		tokenize(t_app *app, char *input);
void	clean_tokenizer(t_app *app);

#endif
