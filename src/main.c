#include "minishell.h"

// Création de l'arbre AST pour tester tous les types de nœuds
t_node_ast *create_test_ast()
{
	t_node_ast *root = malloc(sizeof(t_node_ast));
	root->type = NODE_PIPE;

	// 🎯 Partie gauche : cat < input.txt
	root->left = malloc(sizeof(t_node_ast));
	root->left->type = NODE_R_INPUT;
	root->left->filepath = ft_strdup("input.txt");

	root->left->left = malloc(sizeof(t_node_ast));
	root->left->left->type = NODE_COMMAND;
	root->left->left->args[0] = ft_strdup("/bin/cat");
	root->left->left->args[1] = NULL;
	root->left->left->left = NULL;
	root->left->left->right = NULL;

	// 🎯 Partie droite : grep "test" >> output.txt
	root->right = malloc(sizeof(t_node_ast));
	root->right->type = NODE_R_OUTPUT_APPEND;
	root->right->filepath = ft_strdup("output.txt");

	root->right->left = malloc(sizeof(t_node_ast));
	root->right->left->type = NODE_COMMAND;
	root->right->left->args[0] = ft_strdup("/bin/grep");
	root->right->left->args[1] = ft_strdup("test");
	root->right->left->args[2] = NULL;
	root->right->left->left = NULL;
	root->right->left->right = NULL;

	return root;
}

// 🎯 AST avec heredoc : cat << EOF | wc -w
t_node_ast *create_heredoc_ast()
{
	t_node_ast *root = malloc(sizeof(t_node_ast));

	root->type = NODE_DELIMITER;
	root->delimiter = ft_strdup("EOF");

	root->left = malloc(sizeof(t_node_ast));
	root->left->type = NODE_COMMAND;
	root->left->args[0] = ft_strdup("/bin/cat");
	root->left->args[1] = NULL;
	root->left->left = NULL;
	root->left->right = NULL;

	return root;
}


int	main(int argc, char *argv[], char **envp)
{
	t_app app;
	(void) argc;
	(void) argv;
	if (init_app(&app, envp))
		perror("init app");

	app.first_node = create_test_ast();
	exec_ast(&app, app.first_node);
	// minishell(&app);
	return (0);
}
