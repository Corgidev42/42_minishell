#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFFER_SIZE 128
/*
	access vérifie les permissions par rapport à l’utilisateur effectif du processus, et non l’utilisateur réel.

	Sur certains systèmes, il ne faut pas l’utiliser pour vérifier l’accès avant d’ouvrir un fichier,
	car la situation peut changer entre l’appel à access
	et l’utilisation effective du fichier (problème de conditions de course).
 */
int main() {
	int		fd = open("output.txt", O_RDWR | O_CREAT | O_TRUNC, 0000);
	char	buffer[BUFFER_SIZE];
	ssize_t bytes_read;

	if (fd == -1) {
		perror("open");
		return 1;
	}

	write(fd, "Hello ", 6);
	int fd_copy = dup(fd);
	write(fd_copy, "World!\n", 7); // fd_copy écrit dans le même fichier

	lseek(fd, 0, SEEK_SET);
	write(fd, "Test", 4);
	write(fd_copy, "Test", 4);
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	if (bytes_read < 0)
	{
		perror("read");
		close(fd);
		return 1;
	}
	buffer[bytes_read] = '\0';
	printf("Données lues :\n%s\n", buffer);

	printf("F_OK : Vérifie si le fichier existe. : %d\n", access("output.txt", F_OK));
	printf("R_OK : Vérifie si le fichier existe. : %d\n", access("output.txt", R_OK));
	printf("W_OK : Vérifie si le fichier existe. : %d\n", access("output.txt", W_OK));
	printf("X_OK : Vérifie si le fichier existe. : %d\n", access("output.txt", X_OK));

	dup2(fd, STDOUT_FILENO); // Redirige la sortie standard vers le fichier

    printf("Ce texte est écrit dans output.txt\n");
	unlink("output.txt");
	close(fd);
	return 0;
}

