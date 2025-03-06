#ifndef AST_H
# define AST_H

# include "minishell.h"

enum			e_node_type
{
	NODE_COMMAND,
	NODE_DELIMITER,
	NODE_R_INPUT,
	NODE_R_OUTPUT,
	NODE_R_OUTPUT_APPEND,
	NODE_PIPE
};

struct			s_node_ast
{
	t_node_type			type;
	char				**args;
	char				*filepath;
	char				*delimiter;
	struct s_node_ast	*left;
	struct s_node_ast	*right;
};

t_node_ast	*prepare_ast(t_app *app, int start, int end);
int			exec_ast(t_app *app, t_node_ast *current_node);
void		print_ast(t_node_ast *ast, int level);
void		clean_ast(t_node_ast *node);

#endif
