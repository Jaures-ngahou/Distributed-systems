#include "handling_serveur.h"
#include "download_server.h"

// Déclaration du mutex global
pthread_mutex_t fileMutex;
// fonction update data.txt
void update_data(const char* fichier, const char* ip) {
    void log_message(const char *level, const char *message);
    FILE* readfile = fopen(fichier, "r");
    if(readfile==NULL){
        system("touch data.txt");
        log_message("INFO","data.txt is created");
    }
    FILE* addfile = fopen("temp.txt", "w");
    char ligne[256];
    int delete = 0;

    while (fgets(ligne, sizeof(ligne), readfile)) {// parcourir le fichier ligne apres ligne
        if (delete) {
            if (strncmp(ligne, "IP: ", 4) == 0) {// IP=ligne[4],positif siligne[4]>IP
                delete = 0;
                fputs(ligne, addfile);  
            }
        } else {
            if (strstr(ligne, ip) != NULL) { // verifier si ip est une occcurence de la ligne
                delete = 1;                     //renvoi null si on ne retrouve rien
                continue;
            }
            fputs(ligne, addfile);  
        }
        
    }

    fclose(readfile);
    fclose(addfile);

    remove(fichier);
    rename("temp.txt", fichier);

}



void* handle_client(void* arg) {
    struct KeyValue {
    char key[50];
    int value;
    }   ;
     int clientSocket = *((int*)arg);
    free(arg); // Libérer la mémoire allouée pour le socket
   
while (1) {
 
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    getpeername(clientSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
    int clientPort = ntohs(clientAddr.sin_port);

    char message[250];
    sprintf(message, "User %s is connected on port %d", clientIP, clientPort);
    log_message("INFO", message);

    ssize_t bytesReceived;
    char requestHeader[sizeof(FILE_REQUEST_HEADER)];

        bytesReceived = recv(clientSocket, requestHeader, sizeof(requestHeader), 0);
        if (bytesReceived <= 0) {
            perror("Error receiving request header");
            break;
        }

        if (strncmp(requestHeader, FILE_REQUEST_HEADER, sizeof(FILE_REQUEST_HEADER) - 1) == 0) {
            send_file_contents(clientSocket, "data.txt");
        } else if (strncmp(requestHeader, KV_REQUEST_HEADER, sizeof(KV_REQUEST_HEADER) - 1) == 0) {
            struct KeyValue kv[100];
            bytesReceived = recv(clientSocket, kv, sizeof(kv), 0);
            if (bytesReceived > 0) {
                size_t numPairs = bytesReceived / sizeof(struct KeyValue);
                update_data("data.txt", clientIP);

                FILE *file = fopen("data.txt", "a");
                if (file == NULL) {
                    perror("Error opening file for writing");
                    break;
                }
                
                fprintf(file, "IP: %s    PORT:%d\n", clientIP, clientPort);
                for (size_t i = 0; i < numPairs; i++) {
                    fprintf(file, "%s\t%d\n", kv[i].key, kv[i].value);
                }
                fprintf(file,"\n");
                fclose(file);

                sprintf(message, "Data received from user %s on port %d", clientIP, clientPort);
                log_message("INFO", message);
            } else if (bytesReceived == 0) {
                printf("Client disconnected.\n");
                break;
            } else {
                perror("Error receiving data");
                break;
            }
        }

    //close(clientSocket);

    }

    pthread_exit(NULL);
}



//fi
//gestion des logs
void log_message(const char *level, const char *message) {

  FILE *logFile = fopen("server.log", "a");
    if (logFile == NULL) {
        perror("Erreur lors de l'ouverture du fichier de log");
        return;
    }

    // Obtenir l'heure actuelle
    time_t now = time(NULL);
    struct tm *localTime = localtime(&now);
    char timeString[20];
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localTime);

    // Écrire le message de log dans le fichier
    fprintf(logFile, "[%s] [%s] %s\n", timeString, level, message);

    // Fermer le fichier de log
    fclose(logFile);
}
