#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	int i;

	if (argc > 1)
	{
		i = 1;
		while (argv[i])
		{
			ft_printf("%s", argv[i]);
			if (argv[i + 1])
				ft_printf(" ");
			i++;
		}
	}
	ft_printf("\n");
	return (0);
}