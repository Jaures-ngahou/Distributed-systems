#include "handling_client.h"

struct KeyValue {
    char key[50]; // Utilisez une taille fixe pour la clé
    int value;
};


 // cette fonction cree la socket , charge le contenu du fichier data 
void handle_event( int clientSocket) {   //dans la structure et l'envoi au serveur
     struct KeyValue kv[100];
    int mapSize ;

   
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
    //  close(clientSocket);
}

typedef struct {
    int id;
    char ip[16];
    char filename[100];
    int size;
} FileData;
void send_file_request(int clientSocket) {
    void download_data(char *nom_fichier, char *ip_address);
    ssize_t bytesReceived;
    //char buffer[BUFFER_SIZE];
    char nbre[100];
  

    // Envoyer l'en-tête de la requête de fichier au serveur
    if (send(clientSocket, FILE_REQUEST_HEADER, sizeof(FILE_REQUEST_HEADER), 0) == -1) {
        perror("Erreur lors de l'envoi de l'en-tête de la requête");
        exit(EXIT_FAILURE);
    }
    bytesReceived=recv(clientSocket,nbre,sizeof(nbre),0);
    if(bytesReceived== -1){
        perror("erreur lors de la reception des données");
    }
    int nbre_ligne= atoi(nbre);
    FileData files[nbre_ligne];

    //FileData *files= malloc(nbre_ligne * sizeof(FileData));
    // Recevoir et écrire les données du serveur dans le fichier
    printf("nbr de ligne:%d\n",nbre_ligne);
   
        bytesReceived = recv(clientSocket, files, nbre_ligne * sizeof(FileData), 0) ;
     

    // Vérifier si la boucle s'est terminée en raison d'une erreur ou de la fin de la transmission
    if (bytesReceived == -1) {
        perror("Erreur lors de la réception des données");
        exit(EXIT_FAILURE);
    } else {
        // Toutes les données ont été reçues ou la connexion avec le serveur est fermée
        printf("Toutes les données ont été reçues.\n");
    }
     printf("%-5s %-15s %-30s %s\n", "ID", "Adresse IP", "Nom du fichier", "Taille");
    for (int i = 0; i < nbre_ligne; ++i) {
        if(files[i].size >= 1024){
            if(files[i].size >= 1024*1024){
                 printf("%-5d %-15s %-30s %d Mo\n", files[i].id, files[i].ip, files[i].filename, (files[i].size)/(1024*1024));
             }else{
              printf("%-5d %-15s %-30s %d Ko\n", files[i].id, files[i].ip, files[i].filename, (files[i].size)/1024);
             }
        }else{
        printf("%-5d %-15s %-30s %d o\n", files[i].id, files[i].ip, files[i].filename, files[i].size);
        }
    }
   
    //choisir le fichier à télécharger 
    int no_file;

    printf("+++ veuillez entrer le numero du fichier à télécharger:\t");
    scanf("%d",&no_file);
   // no_file --;
    
    if(no_file <= nbre_ligne && no_file > 0){
        no_file --;
        char *nom_fichier= files[no_file].filename;
        char *ip_address = files[no_file].ip; 
        printf("le telechargement du fichier %s à partir de   %s \n",nom_fichier,ip_address); 
        char lien[300];
        sprintf(lien," curl -O http://%s:8001/%s",ip_address,nom_fichier);
        system(lien);
      //  download_data(nom_fichier,ip_address);
    //  system("curl -O http://%s:8080/%s",nom_fichier,ip_address);

    }else{
        printf("fichier non trouvé\n");
    }
 

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
