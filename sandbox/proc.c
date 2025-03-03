#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

int main() {

// ___________________________________________________________________________
// wait / wait3

	// pid_t pid = fork();

	// if (pid == -1) {
	// 	perror("fork");
	// 	return 1;
	// }

	// if (pid == 0) { // Processus enfant
	// 	printf("L'enfant s'exécute...\n");
	// 	sleep(2);
	// 	printf("L'enfant se termine.\n");
	// 	return 42; // Code de sortie
	// } else { // Processus parent
	// 	printf("test\n");
	// 	int status;
	// 	struct rusage usage;
	// 	wait(&status); // Attend la fin de l'enfant
	// 	// wait3(&status, 0, &usage); //meme chose que wait() mais avec les stats de ressource
	// 	printf("Parent : Mon enfant s'est terminé.\n");
	// 	printf("Temps CPU utilisateur : %ld.%06ld sec\n",
	// 		usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
	// 	printf("Temps CPU système : %ld.%06ld sec\n",
	// 		usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
	// 	printf("Le parent reprend. Code de sortie de l'enfant : %d\n", WEXITSTATUS(status));
	// }

// ___________________________________________________________________________
// waitpid / wait 4


	// pid_t pid = fork();

	// if (pid == -1) {
	// 	perror("fork");
	// 	return 1;
	// }

	// if (pid == 0) { // Processus enfant
	// 	printf("Enfant : Je dors 3 secondes...\n");
	// 	sleep(3);
	// 	printf("Enfant : Fin du processus.\n");
	// 	return 0;
	// } else { // Processus parent
	// 	int status;
	// 	struct rusage usage;
	// 	printf("Parent : J'attends l'enfant...\n");
	// 	wait4(28095, &status, WUNTRACED, &usage); //meme chose que waitpid mais avec les stats de ressources
	// 	// waitpid(pid, &status, 0);
	// 	printf("Parent : Mon enfant s'est terminé.\n");
	// 	printf("Temps CPU utilisateur : %ld.%06ld sec\n",
	// 		usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
	// 	printf("Temps CPU système : %ld.%06ld sec\n",
	// 		usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);

	// 	printf("Parent : L'enfant est terminé !\n");
	// }
	// return 0;

// ___________________________________________________________________________
// execve

	char *args[] = {"/bin/ls", "-l", NULL};
	char *envp[] = {"MY_VAR=42", "HELLO=world", NULL}; // ✅ Tableau d'environnements

	execve(args[0], args, envp); // Passe l'environnement à ls

	perror("execve échoué");
	return 1;

}

/*
wait3 vs wait4 vs waitpid
Fonction		Attendre un enfant		Spécifier un pid ?		Récupérer les ressources ?
wait ()			Oui (n'importe lequel)			X							X
waitpid ( )		Oui								V 							X
wait3()			Oui								X 							V
wait4()			Oui								V 							V

Quand utiliser quoi ?
• wait() → Simple, si tu n'as pas besoin de choisir un enfant précis.
• waitpid (pid, ...) → Quand tu veux attendre un enfant spécifique.
• wait3(...) → Quand tu veux attendre n'importe quel enfant + voir ses ressources.
• wait4 (pid, ...) → Quand tu veux choisir un enfant + voir ses ressources.

📌 TL;DR (Résumé final)
•	Si tu veux juste attendre un enfant → wait()
•	Si tu veux attendre un enfant précis → waitpid()
•	Si tu veux voir combien de CPU/mémoire un enfant a utilisé → wait3()
•	Si tu veux attendre un enfant précis + voir ses ressources → wait4()

*/
