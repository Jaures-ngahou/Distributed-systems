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

struct KeyValue;
void handle_event(char *ip_serveur) ;
void send_file_request(char *ip_serveur);
void format_data(const char *filename) ;

#endif