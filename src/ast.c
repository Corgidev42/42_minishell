#include "minishell.h"

int get_operator_priority(char *token) {
	if (ft_strcmp(token, "<<") == 0)
		return (1);
	if (ft_strcmp(token, "<") == 0)
		return (2);
	if (ft_strcmp(token, ">") == 0 || ft_strcmp(token, ">>") == 0)
		return (3);
	if (ft_strcmp(token, "|") == 0)
		return (4);
	return (-1);
}

t_node_ast	*create_ast_node(t_node_type type, char **args, t_node_ast *left, t_node_ast *right)
{
	t_node_ast	*node = malloc(sizeof(t_node_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = args;
	node->left = left;
	node->right = right;
	return (node);
}

int		prepare_ast(t_app *app, int start, int end)
{
	printf("token : %s\n", app->tokenizer.tokens[start]);
	printf("1.1\n");
	if (start > end)
		return (NULL);

	// Chercher l'opérateur principal en respectant les priorités
	printf("1.2\n");
	int	min_priority = 5;  // max prio
	int	op_index = -1;
	int	i = start;
	printf("start : %d\n", start);
	printf("end : %d\n", end);
	printf("i : %d\n", i);
	while (i < end)
	{
		int priority = get_operator_priority(app->tokenizer.tokens[i]);
		if (priority != -1 && priority <= min_priority)
		{
			min_priority = priority;
			op_index = i;
		}
		i++;
	}
	printf("1.3\n");

	// Si un opérateur a été trouvé, créer le nœud correspondant
	if (op_index != -1)
	{
		t_node_type type;
		printf("1.3.0\n");
		if (ft_strcmp(app->tokenizer.tokens[op_index], "|") == 0)
		{
			type = NODE_PIPE;
			printf("1.3.1\n");
		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], ">") == 0)
		{
			type = NODE_R_OUTPUT;
			printf("1.3.2\n");

		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], ">>") == 0)
		{
			type = NODE_R_OUTPUT_APPEND;
			printf("1.3.3\n");

		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], "<") == 0)
		{
			type = NODE_R_INPUT;
			printf("1.3.4\n");
		}
		else if (ft_strcmp(app->tokenizer.tokens[op_index], "<<") == 0)
		{
			type = NODE_DELIMITER;
			printf("1.3.5\n");

		}
		else
		{
			printf("1.3.6\n");
			return (NULL); // cas où un token de type command n'aurais pas l'index -1
		}
		printf("1.3.7\n");
		char **args = NULL;
		if (type == NODE_R_OUTPUT || type == NODE_R_OUTPUT_APPEND || type == NODE_R_INPUT || type == NODE_DELIMITER)
		{
			if (op_index + 1 <= end)
				args = &app->tokenizer.tokens[op_index + 1];
		}

		int	next_start = op_index + 1;
		if (args)
			next_start += 1;
		return (create_ast_node(type,
								args,
								prepare_ast(app, start, op_index - 1),
								prepare_ast(app, next_start, end)));
	}
	printf("1.4\n");
	// sinon on creer un noeud de type command
	return	create_ast_node(NODE_COMMAND, &app->tokenizer.tokens[start], NULL, NULL);
}

/**
 * Fonction d'affichage pour le debug de l'AST
 */
void print_ast(t_node_ast *ast, int level) {
    if (!ast) return;

    for (int i = 0; i < level; i++)
        printf("  ");

    if (ast->type == NODE_COMMAND)
        printf("CMD: %s\n", ast->args[0]);
    else if (ast->type == NODE_PIPE)
        printf("PIPE\n");
    else if (ast->type == NODE_R_INPUT)
        printf("REDIR IN: %s\n", ast->args ? ast->args[0] : "(null)");
    else if (ast->type == NODE_R_OUTPUT)
        printf("REDIR OUT: %s\n", ast->args ? ast->args[0] : "(null)");
    else if (ast->type == NODE_R_OUTPUT_APPEND)
        printf("APPEND: %s\n", ast->args ? ast->args[0] : "(null)");
    else if (ast->type == NODE_DELIMITER)
        printf("HEREDOC: %s\n", ast->args ? ast->args[0] : "(null)");

    print_ast(ast->left, level + 1);
    print_ast(ast->right, level + 1);
}

int		exec_ast(t_app *app, t_node_ast *current_node)
{
	/*

	type command :
	aller chercher si le premier argument existe d'abortd dans notre bin minishell puis ensuite dans le path
	si tjs pas trouecr, on cherche enfin dans le relatif de la ou on est
	enfin je fais un execve de tous ca,

	type pipe :
	[gestion du pipe] - > on pense a dup2

	type R_INPUT :
	read le filepath et le mettre dans la stdin

	type R_OUTPUT
	write et ecrase le filepath avec la stdout (gestion des droits, creation de fichier si existe aps)

	ty[e R_OUTPUT_APPEND]
	write a la fin du filepath avec la stdout (gestion des droits, creation de fichier si existe aps)

	type DELIMITER :
	tant que le deliminteur n'est pas lu dnas le read_input, je boucle sur read_input
	quand il trouve le delimiteur, il redirige ca vers stdin (on ne prend en compte le delimiteur)

	appel recursif a gauche puis ensuite a droite dans tous les cas tant qu'il y en a.
	penser apres l'exucution de la commande de redcuperer le retour status et le mettre dans app.status
	*/
}

void	clean_ast(t_app *app)
{

}
