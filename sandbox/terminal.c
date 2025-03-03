#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>



// int main() {
//     char *tty = ttyname(STDIN_FILENO);
//     if (tty) {
//         printf("Le terminal est : %s\n", tty);
// 		printf("Numéro du slot du terminal : %d\n", ttyslot());
//     } else {
//         printf("Ce n'est pas un terminal.\n");
//     }


// 	struct winsize w;
//     if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
//         perror("ioctl");
//         return 1;
//     }
//     printf("Taille du terminal : %d lignes x %d colonnes\n", w.ws_row, w.ws_col);

// 	struct termios term;
//     if (tcgetattr(STDIN_FILENO, &term) == -1) {
//         perror("tcgetattr");
//         return 1;
//     }
//     printf("Mode canonique : %s\n", (term.c_lflag & ICANON) ? "Activé" : "Désactivé");



// 	// Récupérer les attributs actuels
//     tcgetattr(STDIN_FILENO, &term);

//     // Désactiver l'écho
//     term.c_lflag &= ~ECHO;
//     tcsetattr(STDIN_FILENO, TCSANOW, &term);

//     // Demander un mot de passe
//     printf("Entrez votre mot de passe : ");
//     char password[100];
//     fgets(password, sizeof(password), stdin);
//     printf("\nMot de passe saisi : %s\n", password);

//     // Rétablir les attributs
//     term.c_lflag |= ECHO;
//     tcsetattr(STDIN_FILENO, TCSANOW, &term);
//     return 0;
// }



#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void set_raw_mode(int enable) {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term); // Récupérer les attributs actuels du terminal

    if (enable) {
        term.c_lflag &= ~(ICANON | ECHO); // Désactiver le mode canonique et l'écho
    } else {
        term.c_lflag |= (ICANON | ECHO);  // Réactiver le mode canonique et l'écho
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &term); // Appliquer les changements
}

int main() {
    char c;

    printf("Mode brut activé ! Tapez des caractères (q pour quitter) :\n");
    set_raw_mode(1); // Activer le mode brut

    while (read(STDIN_FILENO, &c, 1) == 1) {
        printf("Touche pressée : %c (code %d)\n", c, c);
        if (c == 'q') break;
    }

    set_raw_mode(0); // Rétablir le mode canonique
    printf("Mode normal réactivé !\n");

    return 0;
}
