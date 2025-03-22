/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ezeppa <ezeppa@student.42.fr>              #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-03-22 15:25:06 by ezeppa            #+#    #+#             */
/*   Updated: 2025-03-22 15:25:06 by ezeppa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_token(t_tokenizer *tokenizer, char *start, int length)
{
	if (length == 0)
		return ;
	tokenizer->tokens[tokenizer->t_count] = ft_strndup(start, length);
	tokenizer->t_count++;
}

void	handle_env_variable(t_app *app,
	const char **ptr, char *buffer, int *buf_index)
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

void	process_character(const char **ptr, t_help_token *hp)
{
	if (**ptr == '\'' && !(hp->in_double_quote))
		hp->in_single_quote = !(hp->in_single_quote);
	else if (**ptr == '"' && !(hp->in_single_quote))
		hp->in_double_quote = !(hp->in_double_quote);
	else
		hp->buffer[hp->buf_index++] = **ptr;
}

void	init_tokenizer(t_tokenizer *tokenizer, t_help_token *help_token)
{
	tokenizer->tokens = NULL;
	tokenizer->t_count = 0;
	help_token->in_single_quote = 0;
	help_token->in_double_quote = 0;
	help_token->buf_index = 0;
}

void	clean_tokenizer(t_app *app)
{
	int	i;

	i = 0;
	while (app->tokenizer.tokens[i])
	{
		free(app->tokenizer.tokens[i]);
		app->tokenizer.tokens[i] = NULL;
		i++;
	}
	free(app->tokenizer.tokens);
	app->tokenizer.t_count = 0;
}
