#ifndef HANDLING_CLIENT_H
#define HANDLING_CLIENT_H

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

#endif