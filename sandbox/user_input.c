#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

// Fonction asynchrone : affiche un message toutes les 5 secondes
void async_message(int signum) {
	(void)signum; // Éviter le warning unused parameter
	printf("\n🔔 Notification : N'oubliez pas d'enregistrer votre travail !\n");
	rl_on_new_line();   // Indique que l'affichage est perturbé
	rl_redisplay();     // Met à jour la ligne de commande
}

// Fonction asynchrone : remplace la ligne de commande par un message
void	replace_message(int signum) {
	(void)signum;
	rl_replace_line("Hahaha !!!", 0);
	rl_redisplay();
}

// Fonction principale utilisant readline et les autres fonctions
void interactive_shell() {
	char *input;

	// Configurer un timer pour afficher un message toutes les 5 secondes
	signal(SIGALRM, async_message);
	alarm(5);

	while ((input = readline("Prompt> ")) != NULL) {
		if (*input) add_history(input);  // Ajouter à l'historique

		if (strcmp(input, "exit") == 0) {
			free(input);
			break; // Quitte le shell
		}
		
		if (strcmp(input, "replace") == 0) {
			signal(SIGALRM, replace_message);
			alarm(3);
		}

		if (strcmp(input, "clear_history") == 0) {
			rl_clear_history();
		}

		printf("\nCommande exécutée : %s\n", input);
		free(input);
	}
}

int main() {
	printf("Bienvenue dans le shell interactif ! Tapez 'fix' pour corriger votre entrée, 'exit' pour quitter.\n");
	interactive_shell();
	printf("Au revoir !\n");
	return 0;
}
