#ifndef HANDLING_SERVEUR_H
#define HANDLING_SERVEUR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void update_data(const char* fichier, const char* ip) ;
void* handle_client(void* arg);
void log_message(const char *level, const char *message);

#endif