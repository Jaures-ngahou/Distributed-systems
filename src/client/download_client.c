#include "download_client.h"

// void send_file_request(char *serveur_ip){
//      char *ip_serveur= serveur_ip;

//      // Initialisation du socket
//     int clientSocket;
//     struct sockaddr_in serverAddr;

//     // Créez le socket client
//     clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
//     if (clientSocket == -1) {
//         perror("Erreur lors de la création du socket");
//         exit(EXIT_FAILURE);
//     }
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(8080);
//     serverAddr.sin_addr.s_addr = inet_addr(ip_serveur); // Remplacez l'adresse par celle du serveur
//     //serverAddr.sin_addr.s_addr = inet_addr("192.168.43.187");

//     // Connexion au serveur
//     if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
//         perror("Erreur lors de la connexion");
//         exit(EXIT_FAILURE);
//     }

// }
