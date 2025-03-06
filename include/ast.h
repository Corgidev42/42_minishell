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

t_node_ast	*create_ast_node(t_node_type type,
	char **args, char *filepath, char *delimiter);
t_node_ast	*create_ast_node_from_operator(t_app *app, int start, int end, int op_index);
t_node_ast	*create_ast_node_from_command(t_app *app, int start, int end);
t_node_ast	*prepare_ast(t_app *app, int start, int end);
void	clean_ast(t_node_ast *node);

int		exec_ast(t_app *app, t_node_ast *current_node);
void	ast_command(t_app *app, t_node_ast *current_node);
char *search_file(t_app *app, char *command);
void	ast_delimiter(t_app *app, t_node_ast *current_node);
void	ast_r_output_append(t_app *app, t_node_ast *current_node);
void	ast_r_output(t_app *app, t_node_ast *current_node);
void	ast_r_input(t_app *app, t_node_ast *current_node);
void	ast_pipe(t_app *app, t_node_ast *current_node);

#endif
