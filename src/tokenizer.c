#include "minishell.h"

struct s_str_token
{
	int start;
	int	is_ending;
	int	is_quoting;
	int	is_dquoting;
};

char	*ft_strndup(const char *s, size_t n)
{
	char	*str;
	size_t	i;

	if (!(str = (char *)malloc(sizeof(char) * (n + 1))))
		return (NULL);
	i = 0;
	while (i < n)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	size_t	i;

	i = 0;
	while (src[i] && i < len)
	{
		dst[i] = src[i];
		i++;
	}
	while (i < len)
		dst[i++] = '\0';
	return (dst);
}

static void	add_token(t_tokenizer *tokenizer, char *start, int length)
{
	if (length == 0)
		return;
	tokenizer->tokens[tokenizer->t_count] = ft_strndup(start, length);
	tokenizer->t_count++;
}

char *minishell_getenv(t_app *app, char *str)
{
	int i = 0;
	if (ft_strcmp(str, "?") == 0)
	{
		return (ft_itoa(app->status));
	}
	while (app->envp[i])
	{
		if (ft_strncmp(app->envp[i], str, ft_strlen(str)) == 0)
			return (app->envp[i] + ft_strlen(str) + 1);
		i++;
	}
	return (NULL);
}


static void	handle_env_variable(t_app *app, const char **ptr, char *buffer, int *buf_index)
{
	const char	*var_start;
	char		var_name[256];
	char		*env_value;

	var_start = *ptr + 1;
	while (isalnum(*var_start) || *var_start == '_' || *var_start == '?')
		var_start++;
	ft_strncpy(var_name, *ptr + 1, var_start - (*ptr + 1));
	var_name[var_start - (*ptr + 1)] = '\0';
	env_value = minishell_getenv(app, var_name);
	if (env_value)
	{
		ft_strncpy(buffer + *buf_index, env_value, ft_strlen(env_value));
		*buf_index += ft_strlen(env_value);
	}
	*ptr = var_start - 1;
}

static void	process_character(const char **ptr, char *buffer, int *buf_index,
					int *in_single_quote, int *in_double_quote)
{
	if (**ptr == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
	else if (**ptr == '"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
	else
		buffer[(*buf_index)++] = **ptr;
}
int		tokenize(t_app *app, char *input)
{
	t_tokenizer	tokenizer = { .t_count = 0 };
	int			in_single_quote = 0, in_double_quote = 0;
	char		buffer[4096];
	int			buf_index = 0;

	tokenizer.tokens = (char **)malloc(sizeof(char *) * 4096);
	if (!tokenizer.tokens)
		return (1);
	for (int i = 0; i < 4096; i++)
		tokenizer.tokens[i] = NULL;
	for (const char *ptr = input; *ptr; ptr++)
	{
		if (!in_single_quote && !in_double_quote && isspace(*ptr))
		{
			if (buf_index > 0)
			{
				buffer[buf_index] = '\0';
				add_token(&tokenizer, buffer, buf_index);
				buf_index = 0;
			}
		}
		else if (*ptr == '$' && !in_single_quote)
			handle_env_variable(app, &ptr, buffer, &buf_index);
		else
			process_character(&ptr, buffer, &buf_index, &in_single_quote, &in_double_quote);
	}
	if (buf_index > 0)
	{
		buffer[buf_index] = '\0';
		add_token(&tokenizer, buffer, buf_index);
	}
	app->tokenizer = tokenizer;
	return (0);
}

void	clean_tokenizer(t_app *app)
{
	int i = 0;
	while (app->tokenizer.tokens[i])
	{
		free(app->tokenizer.tokens[i]);
		app->tokenizer.tokens[i] = NULL;
		i++;
	}
	free(app->tokenizer.tokens);
	app->tokenizer.t_count = 0;
}
