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
#include "download_client.h"




struct KeyValue;
void handle_event(int clientSocket) ;
void send_file_request( int clientSocket);

#endif