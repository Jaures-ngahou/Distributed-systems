

#include "download_server.h"
 



void send_file_contents(int sock, const char *filetext) {

    int serialize_file_data(FileData *files, int file_count, char **serialized_data) ;

    FILE *file = fopen("data.txt", "r");
    if (file == NULL) {
        fprintf(stderr,"Erreur lors de l'ouverture du fichier");
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
                fprintf(stderr,"Erreur lors de l'allocation de mémoire");
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

    char *serialized_data;
    int serialized_size = serialize_file_data(files, file_count, &serialized_data);
   

    // Affichage des données formatées
    // printf("%-5s %-15s %-30s %s\n", "ID", "Adresse IP", "Nom du fichier", "Taille");
    // for (int i = 0; i < file_count; ++i) {
    //     printf("%-5d %-15s %-30s %d\n", files[i].id, files[i].ip, files[i].filename, files[i].size);
    // }
    if (send(sock, &serialized_size, sizeof(int), 0) == -1) {
   
    
        fprintf(stderr,"error while sending data");
        exit(-1);
    } 

    if (send(sock, serialized_data, serialized_size, 0) == -1) {
    fprintf(stderr, "Erreur lors de l'envoi des données\n");
    free(serialized_data);
   // return -1;
}

free(serialized_data);
  
    fflush(stdout);
    printf("data send \n");
    free(files);
    fclose(file);
   // return 0;
}

// serialisation de la structure
int serialize_file_data(FileData *files, int file_count, char **serialized_data) {
    int total_size = sizeof(int) + file_count * sizeof(FileData);
    *serialized_data = (char *)malloc(total_size);
    if (*serialized_data == NULL) {
        fprintf(stderr, "Erreur lors de l'allocation de mémoire\n");
       // return -1;
    }

    char *ptr = *serialized_data;
    memcpy(ptr, &file_count, sizeof(int));
    ptr += sizeof(int);

    for (int i = 0; i < file_count; i++) {
        memcpy(ptr, &files[i], sizeof(FileData));
        ptr += sizeof(FileData);
    }

    return total_size;
}


