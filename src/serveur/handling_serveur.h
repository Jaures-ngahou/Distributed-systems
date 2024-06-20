#ifndef HANDLING_SERVEUR_H
#define HANDLING_SERVEUR_H

#define FILE_REQUEST_HEADER "FILE_REQUEST"
#define KV_REQUEST_HEADER "KV_REQUEST"
#define BUFFER_SIZE 1024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>


// fonction qui permet de mettre à jour le fichier data.txt
void update_data(const char* fichier, const char* ip) ;

// fonction executée par chaque thread. avec deux actions en fonctions des deux types de requete reçues
void* handle_client(void* arg);
void log_message(const char *level, const char *message);
// void send_file_contents(int sock, const char *filename) ;
#endif