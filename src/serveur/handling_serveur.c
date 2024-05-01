#include "handling_serveur.h"


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
// Fonction executée par chaque thread



void* handle_client(void* arg) {
        // Initialiser le mutex
    pthread_mutex_init(&fileMutex, NULL);
    void update_data(const char* fichier, const char* ip) ;
    void log_message(const char *level, const char *message);
     char requestHeader[sizeof(FILE_REQUEST_HEADER)];
    struct KeyValue {
        char key[50];
        int value;
    };

    int clientSocket = *((int*)arg);
    free(arg); // Libérer la mémoire allouée pour le socket

    struct KeyValue kv[100];
    ssize_t bytesReceived;
    char *fichier= "data.txt";
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
    //envoi du fichier data.txt au client
      bytesReceived = recv(clientSocket, requestHeader, sizeof(requestHeader), 0);
    if (bytesReceived <= 0) {
        perror("Erreur lors de la réception de l'en-tête de la requête");
        pthread_exit(NULL);
    } // envoi du fichier data.txt au client
    if (strncmp(requestHeader, FILE_REQUEST_HEADER, sizeof(FILE_REQUEST_HEADER) - 1) == 0) {
        // Requête de fichier
        send_file_contents(clientSocket, "data.txt");
        
    }
    if (strncmp(requestHeader, KV_REQUEST_HEADER, sizeof(KV_REQUEST_HEADER) - 1) == 0) {
        
        // Recevoir les données du client
        bytesReceived = recv(clientSocket, kv, sizeof(kv), 0);
        if (bytesReceived > 0) {
            // Calculer le nombre de paires de clé-valeur reçues
            size_t numPairs = bytesReceived / sizeof(struct KeyValue);
        update_data(fichier, clientIP); 
        

            // Ouvrir le fichier pour écrire les données
            FILE *file = fopen("data.txt", "a");
            
                // Écrire les informations du client dans le fichier
                fprintf(file, "IP: %s    PORT:%d\n", clientIP, clientPort);
                for (size_t i = 0; i < numPairs; i++) {
                
                    fprintf(file, "%s      %d\n", kv[i].key, kv[i].value);
                }
                fprintf(file,"\n");
                fclose(file);
            sprintf(message,"data received from user %s on port  %d",clientIP,clientPort);
     
              log_message("INFO",message );

        
        } else if (bytesReceived == 0) {
            printf("Client déconnecté.\n");
        } else {
            perror("Erreur lors de la réception des données");
        }
    }
 // Déverrouiller le mutex après avoir accédé au fichier
        pthread_mutex_unlock(&fileMutex);
    // Fermer le socket client
    close(clientSocket);
        // Nettoyer le mutex avant de terminer le programme
    pthread_mutex_destroy(&fileMutex);

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

//envoi du fichier data.txt
void send_file_contents(int sock, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    // Lecture et envoi du contenu du fichier ligne par ligne
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (send(sock, buffer, bytesRead, 0) == -1) {
            perror("Erreur lors de l'envoi du contenu du fichier");
            exit(EXIT_FAILURE);
        }
    }

    if (bytesRead < 0) {
        perror("Erreur lors de la lecture du fichier");
        exit(EXIT_FAILURE);
    }

    // Fermeture du fichier après l'envoi
    fclose(file);
}