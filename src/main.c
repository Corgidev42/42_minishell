#include "minishell.h"

// int	main(int argc, char *argv[], char **envp)
// {
// 	t_app app;
// 	(void) argc;
// 	(void) argv;
// 	if (init_app(&app, envp))
// 		perror("init app");
// 	minishell(&app);
// 	return (0);
// }

int main()
{
    t_app app;
    char *tokens[] = {"ls", "-l", "|", "grep", ".c", ">", "output.txt", NULL};
    app.tokenizer.tokens = tokens;

    t_node_ast *ast = prepare_ast(&app, 0, 6);
    print_ast(ast, 0);
    return 0;
}
