#ifndef DOWNLOAD_CLIENT_H
#define DOWNLOAD_CLIENT_H
#include "handling_client.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>


int download_data_client( char *nom_fichier,char *ip_address, int file_size1);
int  download_data_server(int port);
void showProgressBar(int current, int total) ;

#endif
