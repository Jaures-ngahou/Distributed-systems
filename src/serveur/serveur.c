#include "handling_serveur.h"






int main() {
    // Initialisation du socket serveur
    int serverSocket;
    struct sockaddr_in serverAddr;



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


    return 0;
}
