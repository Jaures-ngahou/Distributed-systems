#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000
#define MAX_IP_LENGTH 16
#define MAX_FILENAME_LENGTH 100

// Structure pour stocker les informations de chaque fichier
typedef struct {
    int id;
    char ip[MAX_IP_LENGTH];
    char filename[MAX_FILENAME_LENGTH];
    long size;
} FileData;

int main() {
    FILE *file = fopen("datastamp.txt", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    FileData files[100]; // Supposons que vous ne dépassiez pas 100 fichiers
    int file_count = 0;

    int current_id = 0;
    char current_ip[MAX_IP_LENGTH] = "";
    char current_filename[MAX_FILENAME_LENGTH] = "";
    long current_size = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "IP:", 3) == 0) {
            sscanf(line, "IP: %s", current_ip);
        } else if (sscanf(line, "%*s %d", &current_id) == 1) {
            sscanf(line, "%s %ld", current_filename, &current_size);
            // Création de la structure pour stocker les informations du fichier
            FileData file_data;
            file_data.id = file_count;
            strcpy(file_data.ip, current_ip);
            strcpy(file_data.filename, current_filename);
            file_data.size = current_size;
            files[file_count++] = file_data;
		//ij ++;
        }
    }

    // Affichage des données formatées
    printf("%-5s %-15s %-30s %s\n", "ID", "Adresse IP", "Nom du fichier", "Taille");
    for (int i = 0; i < file_count; ++i) {
        printf("%-5d %-15s %-30s %ld\n", files[i].id, files[i].ip, files[i].filename, files[i].size);
    }

    fclose(file);
    return 0;
}
