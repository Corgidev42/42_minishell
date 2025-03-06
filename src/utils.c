#include "minishell.h"

char	*ft_strndup(const char *s, size_t n)
{
	char	*str;
	size_t	i;

	str = (char *)malloc(sizeof(char) * (n + 1));
	if (!str)
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

char	*minishell_getenv(t_app *app, char *str)
{
	int	i;

	i = 0;
	if (ft_strcmp(str, "?") == 0)
	{
		return (ft_itoa(app->status));
	}
	while (app->envp[i])
	{
		if (ft_strncmp(app->envp[i], str, ft_strlen(str)) == 0)
			if (app->envp[i][ft_strlen(str)] == '=')
				return (app->envp[i] + ft_strlen(str) + 1);
		i++;
	}
	return (NULL);
}

int	get_operator_priority(char *token)
{
	if (ft_strcmp(token, "<<") == 0)
		return (3);
	if (ft_strcmp(token, "<") == 0)
		return (3);
	if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">>") == 0)
		return (2);
	if (ft_strcmp(token, "|") == 0)
		return (1);
	return (-1);
}