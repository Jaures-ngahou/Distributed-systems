#include "download_server.h"
 

#define MAX_LINE_LENGTH 200
#define MAX_IP_LENGTH 16
#define MAX_FILENAME_LENGTH 100

// Structure pour stocker les informations de chaque fichier
typedef struct {
    int id;
    char ip[MAX_IP_LENGTH];
    char filename[MAX_FILENAME_LENGTH];
    int size;
} FileData;

void send_file_contents(int sock, const char *filetext) {
    FILE *file = fopen("data.txt", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(-1);
    }

    char line[MAX_LINE_LENGTH];
    FileData *files = NULL; // Déclaration d'un pointeur pour le tableau dynamique
    int file_count = 0;
    int id1=1;
    int current_id = 0;
    char current_ip[MAX_IP_LENGTH] = "";
    char current_filename[MAX_FILENAME_LENGTH] = "";
    int current_size = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "IP:", 3) == 0) {
            sscanf(line, "IP: %s", current_ip);
        } else if (sscanf(line, "%*s %d", &current_id) == 1) {
            sscanf(line, "%s %d", current_filename, &current_size);
            // Allocation dynamique de mémoire pour la nouvelle entrée de fichier
            files = realloc(files, (file_count + 1) * sizeof(FileData));
            if (files == NULL) {
                perror("Erreur lors de l'allocation de mémoire");
                exit(-1);
            }
            // Création de la structure pour stocker les informations du fichier
            FileData file_data;
            file_data.id = id1;
            strcpy(file_data.ip, current_ip);
            strcpy(file_data.filename, current_filename);
            file_data.size = current_size;
            files[file_count++] = file_data;
            id1 ++;
        }
    }
    char nbre[100];
    sprintf(nbre,"%d",file_count);
    // Affichage des données formatées
    // printf("%-5s %-15s %-30s %s\n", "ID", "Adresse IP", "Nom du fichier", "Taille");
    // for (int i = 0; i < file_count; ++i) {
    //     printf("%-5d %-15s %-30s %d\n", files[i].id, files[i].ip, files[i].filename, files[i].size);
    // }
    if(send(sock,nbre,sizeof(nbre),0) == -1){
        perror("erreur lors de l'envoi des donnés");
        exit(-1);
    } 
    if(send(sock,files,file_count * sizeof(FileData),0) == -1){
        perror("erreur lors de l'envoi des donnés");
        exit(-1);
    } 
    fflush(stdout);
    printf("liste envoyées\n");
    free(files);
    fclose(file);
   // return 0;
}
