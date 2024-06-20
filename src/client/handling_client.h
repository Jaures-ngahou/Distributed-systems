#ifndef HANDLING_CLIENT_H
#define HANDLING_CLIENT_H
#define KV_REQUEST_HEADER "KV_REQUEST"
#define FILE_REQUEST_HEADER "FILE_REQUEST"
#define BUFFER_SIZE 1024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/inotify.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <errno.h>
#include <time.h>
#include <net/if.h>
#include "download_client.h"

#define INTERVAL 3 // Intervalle en secondes pour mesurer le débit



struct KeyValue;

typedef struct {
    int id;
    char ip[16];
    char filename[100];
    int size;
} FileData;

void handle_event(int clientSocket) ;
//fonction qui permet de recuperer la liste des fichiers disponibles dans le reseau
void send_file_request( int clientSocket);
//
//fonction qui prend en parametre le nom d'un fichier et retourne sa taille
long get_lenghtFile(char *nom_fichier);
struct net_stat;
void get_net_stat(struct net_stat *stats, int *count);
// permet de calculer la vitesse de transmission
unsigned long get_tx_rate(const char *interface);
int deserialize_file_data(char *serialized_data, FileData **files) ;



#endif