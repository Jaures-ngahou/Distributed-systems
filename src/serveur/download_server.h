#ifndef DOWNLOAD_SERVER_H
#define  DOWNLOAD_SERVER_H
#include "handling_serveur.h"

// struct fileData_s;
// typedef struct fileData_s fileData;
void send_file_contents(int sock, const char *filetext) ;

#endif