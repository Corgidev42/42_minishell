#ifndef BUILT_IN_H
# define BUILT_IN_H

# include "minishell.h"

int	is_built_in_function(t_node_ast *ast);
int	exec_built_in(t_app *app, t_node_ast *ast);

#endif