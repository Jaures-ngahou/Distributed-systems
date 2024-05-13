#include "handling_client.h"
#include "download_client.h"



int main(int argc, char **argv) {
    if(argc < 2){
        printf("syntaxe: ./client ip\n");
        exit(-1);
    }
    int option;
    char *ip_serveur= argv[1];

     // Initialisation du socket
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Créez le socket client
    clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (clientSocket == -1) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr(ip_serveur); // Remplacez l'adresse par celle du serveur
    //serverAddr.sin_addr.s_addr = inet_addr("192.168.43.187");

    // Connexion au serveur
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Erreur lors de la connexion");
        exit(EXIT_FAILURE);
    }

    printf("++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("++ Welcome to our file sharing application  ++\n");
    printf("++++++++++++++++++++++++++++++++++++++++++++++\n\n");
    printf("To display the list of file stored on the server, press 1\n");
    printf("To upload your files contained in \" data\" folder, press 2\n");
    printf("To update your files on the server, press 3\n");
    printf("To download files from server, press 4\n ");
    printf("To quit our file sharing application press 0\n ");
  
    while(1){

        scanf("%d",&option);
        switch(option){
            case 0:
              //  printf("sorry, it's available in prenium version\n");
                printf("Goodbye !!\n");
                 close(clientSocket);
                exit(0);
            break;
            case 1:
                printf("Fichiers disponibles\n");
                send_file_request(clientSocket);

            break;
            case 2:
                handle_event(clientSocket);
            break;
            case 3:
                handle_event(clientSocket);
                printf("data update \n");
            break;
            case 4:
                printf("sorry, it's available in prenium version\n");
            break;
            default:
            printf("Please, enter a valid number\t [0;1;2;3;4] \n");
            break;
        }
        // if(i!=4)
         printf("\n what do you want else:");
        // i++;
    }

          
   // close(clientSocket);
    


    return 0;
}

