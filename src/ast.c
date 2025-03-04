#include "minishell.h"

int		prepare_ast(t_app *app, int start, int end)
{
	/*
	1er bloc :
	je parocours tous mes tokens et defini mes priorite en fonction des operateur et leurs index, si il n'en a pas, c'est -1

	2e bloc :
	je prend l'index postif
	je creer mon node avec son status correspodnant
	si type R_X, prendre l'argument de droite comme filepath
	si type DELIMITER, prendre l'argument de droite comme delimiteur
	et je creer 2 branche, gauche qui aura pour start : start, et end : index_token - 1
	droite qui aura pour start : index_token + 1(+2 si delimiteur ou redirect) et end : end;

	3e bloc :
	je creer un node de type commande
	il prend le premier, et il met de start a index comme argument
	*/
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
