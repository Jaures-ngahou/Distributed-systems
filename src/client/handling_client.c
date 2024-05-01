#include "handling_client.h"

struct KeyValue {
    char key[50]; // Utilisez une taille fixe pour la clé
    int value;
};


 // cette fonction cree la socket , charge le contenu du fichier data 
void handle_event(char *ip_serveur) {   //dans la structure et l'envoi au serveur
     struct KeyValue kv[100];
    int mapSize ;

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
    // Créez un tableau pour stocker les données à envoyer
   
     mapSize = 0;

    // Ouvrez le dossier "./data/"
    DIR *dir = opendir("./data");
    if (dir == NULL) {
        perror("Erreur lors de l'ouverture du dossier");
        exit(EXIT_FAILURE);
    }

    // Parcourez les fichiers dans le dossier
    struct dirent *entry;
    struct stat file_info;
    while ((entry = readdir(dir)) != NULL) { //retourne un pointeur vers le prochqin fichier
        if (entry->d_type == DT_REG) { // verifier si c'est un fichier
            char file_path[300];
            snprintf(file_path, sizeof(file_path), "./data/%s", entry->d_name);

            if (stat(file_path, &file_info) == 0) {
                // Remplissez les données dans kv
                strncpy(kv[mapSize].key, entry->d_name, sizeof(kv[mapSize].key));
                kv[mapSize].value = file_info.st_size;
                mapSize++;
            }
        }
    }
    for(int i=0; i< mapSize;i++)
        printf("files: %s size %d \n",kv[i].key,kv[i].value);
    // Fermez le dossier
    closedir(dir);
    // envoi de l'entete pour structure kv
     if (send(clientSocket, KV_REQUEST_HEADER, sizeof(KV_REQUEST_HEADER), 0) == -1) {
        perror("Erreur lors de l'envoi de l'en-tête de la requête");
        exit(EXIT_FAILURE);
    }
    sleep(1);
    // Envoyez uniquement les données dans `kv` qui ont été remplies (de taille `mapSize`)
    if (send(clientSocket, kv, mapSize * sizeof(struct KeyValue), 0) == -1) {
        perror("Erreur lors de l'envoi des données");
        exit(EXIT_FAILURE);
    }

    printf("Data sent to server.\n");
     close(clientSocket);
}
// envoi et reception du fichier data.txt
void send_file_request(char *ip_serveur){
      // Initialisation du socket
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];
    ssize_t bytesReceived;
     FILE *file = fopen("data.txt", "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier data.txt");
        exit(EXIT_FAILURE);
    }

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
     // Envoyer l'en-tête de la requête de fichier au serveur
    if (send(clientSocket, FILE_REQUEST_HEADER, sizeof(FILE_REQUEST_HEADER), 0) == -1) {
        perror("Erreur lors de l'envoi de l'en-tête de la requête");
        exit(EXIT_FAILURE);
    }  while ((bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
        // Afficher le contenu du fichier sur le terminal
        fwrite(buffer, 1, bytesReceived, file);
       // fwrite(buffer, 1, bytesReceived, stdout);
        
    }
    if (fclose(file) != 0) {
    perror("Erreur lors de la fermeture du fichier");
    exit(EXIT_FAILURE);
}
    if (bytesReceived < 0) {
        perror("Erreur lors de la réception du fichier");
    }
    
      close(clientSocket);
void format_data(const char *filename) ;

format_data("data.txt");

}

//formatage

void format_data(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char line[BUFFER_SIZE];
    char ip[20];
    char port[10];
    int size;

    while (fgets(line, BUFFER_SIZE, file) != NULL) {
        if (strstr(line, "IP:") != NULL && strstr(line, "PORT:") != NULL) {
            sscanf(line, "IP: %s PORT:%s", ip, port);
           // printf("IP: %s    PORT:%s\n", ip, port);
        } else {
            sscanf(line, "%*s %d", &size);
            if (size >= 1024) {
                printf("%s      %.2f MB\n", strtok(line, " "), size / 1024.0);
            } else {
                printf("%s      %d KB\n", strtok(line, " "), size);
            }
        }
        size=0;
    }

    fclose(file);
}
