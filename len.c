#include <stdio.h>
#include <unistd.h>  // Pour sleep()
#include <string.h>  // Pour memset()

void print_progress_bar(int percentage) {
    int bar_width = 50;  // Largeur de la barre de progression
    int pos = (percentage * bar_width) / 100;

    printf("[");
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) printf("=");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %d%%\r", percentage);
    fflush(stdout);  // Force l'affichage immédiat
}

int main() {
    int download_size = 100;  // Taille totale à télécharger (pour la simulation)
    
    printf("Téléchargement en cours :\n");
    for (int i = 0; i <= download_size; ++i) {
        print_progress_bar(i);
        usleep(50000);  // Attendre 50 ms pour simuler le téléchargement
    }
    printf("\nTéléchargement terminé !\n");

    return 0;
}


/*
#include <stdio.h>
#include <unistd.h>  // Pour sleep()

int main() {
    for (int i = 0; i <= 5; i++) {
        // Retourne au début de la ligne et efface la ligne
        printf("\r\033[KCompteur : %d", i);
        fflush(stdout);  // Force l'affichage immédiat
        sleep(1);  // Attend 1 seconde
    }

    // Ajoute une nouvelle ligne à la fin pour éviter d'écraser la dernière mise à jour
    printf("\n");

    return 0;
}
*/
