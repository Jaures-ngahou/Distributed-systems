#ifndef DOWNLOAD_SERVER_H
#define  DOWNLOAD_SERVER_H
#include "handling_serveur.h"

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
// struct fileData_s;
// typedef struct fileData_s fileData;
// fonction qui permet d'envoyer le contenu du fichier data.txt au client
void send_file_contents(int sock, const char *filetext) ;
int serialize_file_data(FileData *files, int file_count, char **serialized_data) ;


#endif