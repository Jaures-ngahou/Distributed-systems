#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/inotify.h>
#include <dirent.h>
#include <sys/stat.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16)) // taille max

struct KeyValue {
    char key[50]; // Utilisez une taille fixe pour la clé
    int value;
};

void handle_event() {
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
    serverAddr.sin_addr.s_addr = inet_addr("192.168.43.187"); // Remplacez l'adresse par celle du serveur

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
        printf("clés: %s valeur %d \n",kv[i].key,kv[i].value);
    // Fermez le dossier
    closedir(dir);

    // Envoyez uniquement les données dans `kv` qui ont été remplies (de taille `mapSize`)
    if (send(clientSocket, kv, mapSize * sizeof(struct KeyValue), 0) == -1) {
        perror("Erreur lors de l'envoi des données");
        exit(EXIT_FAILURE);
    }

    printf("Données envoyées au serveur.\n");
     close(clientSocket);
}

int main() {
    int fd, wd;
    char buffer[BUF_LEN];
    handle_event();
   
    
    //handle_event(clientSocket);
    // Initialisation d'Inotify
    fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        exit(EXIT_FAILURE);
    }

    // Ajout d'une surveillance sur le dossier ./data/
    wd = inotify_add_watch(fd, "./data", IN_CREATE | IN_DELETE | IN_MODIFY |IN_MOVE_SELF |IN_MOVED_FROM| IN_MOVED_TO);
    if (wd < 0) {
        perror("inotify_add_watch");
        exit(EXIT_FAILURE);
    }

    printf("Surveillance des modifications...\n");

    // Boucle d'écoute continue
    while (1) {
        int length = read(fd, buffer, BUF_LEN); // retourne le nombre d'octets lus
        if (length < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        struct inotify_event *event;
        for (char *ptr = buffer; ptr < buffer + length; ptr += EVENT_SIZE + event->len) {//tampon notify
            event = (struct inotify_event *)ptr;

            if (event->mask & (IN_CREATE | IN_DELETE | IN_MODIFY|IN_MOVE_SELF |IN_MOVED_FROM| IN_MOVED_TO)) {
                handle_event();
            }
        }

    }

    // Nettoyage
    inotify_rm_watch(fd, wd);
    close(fd);
   

    return 0;
}

