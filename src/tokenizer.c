#include "minishell.h"

void	process_tokenize(t_app *app,
	char *input, t_tokenizer *tokenizer, t_help_token *hp)
{
	const char	*ptr;

	ptr = input;
	while (*ptr)
	{
		if (!hp->in_single_quote && !hp->in_double_quote && isspace(*ptr))
		{
			if (hp->buf_index > 0)
			{
				hp->buffer[hp->buf_index] = '\0';
				add_token(tokenizer, hp->buffer, hp->buf_index);
				hp->buf_index = 0;
			}
		}
		else if (*ptr == '$' && !hp->in_single_quote)
			handle_env_variable(app, &ptr, hp->buffer, &hp->buf_index);
		else
			process_character(&ptr, hp);
		ptr++;
	}
}

int	tokenize(t_app *app, char *input)
{
	t_tokenizer		tokenizer;
	t_help_token	hp;
	int				i;

	init_tokenizer(&tokenizer, &hp);
	tokenizer.tokens = (char **)malloc(sizeof(char *) * 4096);
	if (!tokenizer.tokens)
		return (1);
	i = 0;
	while (i < 4096)
	{
		tokenizer.tokens[i] = NULL;
		i++;
	}
	process_tokenize(app, input, &tokenizer, &hp);
	if (hp.buf_index > 0)
	{
		hp.buffer[hp.buf_index] = '\0';
		add_token(&tokenizer, hp.buffer, hp.buf_index);
	}
	app->tokenizer = tokenizer;
	return (0);
}
