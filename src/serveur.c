#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

struct KeyValue {
    char key[50];
    int value;
};

// Déclaration du mutex global
pthread_mutex_t fileMutex;

void* handle_client(void* arg) {
void log_message(const char *level, const char *message);

    int clientSocket = *((int*)arg);
    free(arg); // Libérer la mémoire allouée pour le socket

    struct KeyValue kv[100];
    ssize_t bytesReceived;

    // Obtenir l'adresse IP et le port du client
    struct sockaddr_in clientAddr; // stocker les infos du client
    socklen_t clientAddrLen = sizeof(clientAddr); //taille de la structure
    getpeername(clientSocket, (struct sockaddr*)&clientAddr, &clientAddrLen); //adresse ip et port
    char clientIP[INET_ADDRSTRLEN];// declaration d'un tableau pour stocker une addr ip
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);//convertir en notation decimale 
    int clientPort = ntohs(clientAddr.sin_port);
    char message[250];
    sprintf(message,"user %s is connected on port %d",clientIP,clientPort);
     // Verrouiller le mutex avant d'accéder au fichier partagé
        pthread_mutex_lock(&fileMutex);
    log_message("INFO",message );

    // Recevoir les données du client
    bytesReceived = recv(clientSocket, kv, sizeof(kv), 0);
    if (bytesReceived > 0) {
        // Calculer le nombre de paires de clé-valeur reçues
        size_t numPairs = bytesReceived / sizeof(struct KeyValue);

       

        // Ouvrir le fichier pour écrire les données
        FILE *file = fopen("data.txt", "a");
        if (file == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
        } else {
            // Écrire les informations du client dans le fichier
            fprintf(file, "%s    %d\n", clientIP, clientPort);
            for (size_t i = 0; i < numPairs; i++) {
               
                fprintf(file, "%s      %d\n", kv[i].key, kv[i].value);
            }
            fprintf(file,"\n");
            fclose(file);
        }

       
    } else if (bytesReceived == 0) {
        printf("Client déconnecté.\n");
    } else {
        perror("Erreur lors de la réception des données");
    }
 // Déverrouiller le mutex après avoir accédé au fichier
        pthread_mutex_unlock(&fileMutex);
    // Fermer le socket client
    close(clientSocket);
    pthread_exit(NULL);
}
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
int main() {
    // Initialisation du socket serveur
    int serverSocket;
    struct sockaddr_in serverAddr;

    // Initialiser le mutex
    pthread_mutex_init(&fileMutex, NULL);

    // Créer le socket serveur
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Erreur lors de la création du socket");
         log_message("DEBUG", "Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Lier le socket serveur à l'adresse
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Erreur lors de la liaison");
        log_message("DEBUG", "binding error");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Écouter les connexions entrantes
    if (listen(serverSocket, 50) == -1) {
        perror("Erreur lors de l'écoute");
         log_message("INFO", "binding error");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    printf("Serveur en écoute sur le port 8080...\n");

    // Boucle pour accepter les connexions
    while (1) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int* clientSocketPtr = malloc(sizeof(int));
        *clientSocketPtr = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);

        if (*clientSocketPtr == -1) {
            perror("Erreur lors de l'acceptation");
             log_message("INFO", "binding error");
            free(clientSocketPtr);
            continue;
        }

        // Créer un thread pour gérer la connexion client
        pthread_t clientThread;
        if (pthread_create(&clientThread, NULL, handle_client, clientSocketPtr) != 0) {
            perror("Erreur lors de la création du thread");
             log_message("DEBUG", "binding error");
            close(*clientSocketPtr);
            free(clientSocketPtr);
        }

        // Détacher le thread pour ne pas attendre sa fin
        pthread_detach(clientThread);
    }

    // Fermeture du socket serveur
    close(serverSocket);

    // Nettoyer le mutex avant de terminer le programme
    pthread_mutex_destroy(&fileMutex);

    return 0;
}
