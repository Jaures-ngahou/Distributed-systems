#include "handling_client.h"

struct KeyValue {
    char key[100]; // Utilisez une taille fixe pour la clé
    int value;
};


 // cette fonction cree la socket , charge le contenu du fichier data 
void handle_event( int clientSocket) {   //dans la structure et l'envoi au serveur
     struct KeyValue kv[100];
    int mapSize ;
//
    const char *interface = "wlp1s0";
    unsigned long tx_rate = get_tx_rate(interface);
    printf("TX Rate for interface %s: %lu bytes/sec\n", interface, tx_rate);
   char msg[20];
   sprintf(msg,"%s:%lu",interface,tx_rate);
    // Créez un tableau pour stocker les données à envoyer
   
     mapSize = 0;

    // Ouvrez le dossier "./data/"
    DIR *dir = opendir("./data");
    if (dir == NULL) {
        fprintf(stderr,"Erreur lors de l'ouverture du dossier");
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
        fprintf(stderr,"Erreur lors de l'envoi de l'en-tête de la requête");
        exit(EXIT_FAILURE);
    }
    sleep(1);
    // Envoyez uniquement les données dans `kv` qui ont été remplies (de taille `mapSize`)
    if (send(clientSocket, kv, mapSize * sizeof(struct KeyValue), 0) == -1) {
        fprintf(stderr,"Erreur lors de l'envoi des données");
        exit(EXIT_FAILURE);
    }

//     }
    printf("Data sent to server.\n");
    //  close(clientSocket);
}



void send_file_request(int clientSocket) {

int  download_data_client(char *nom_fichier, char *ip_address,int file_size);
    
   // ssize_t bytesReceived;
        

    // Envoyer l'en-tête de la requête de fichier au serveur
    if (send(clientSocket, FILE_REQUEST_HEADER, sizeof(FILE_REQUEST_HEADER), 0) == -1) {
        fprintf(stderr,"Erreur lors de l'envoi de l'en-tête de la requête");
        exit(EXIT_FAILURE);
    }
  //jo
  // Réception de la taille du paquet
int serialized_size;
if (recv(clientSocket, &serialized_size, sizeof(int), 0) == -1) {
    fprintf(stderr, "Erreur lors de la réception de la taille du paquet\n");
    exit(EXIT_FAILURE);
}

// Réception des données
char *serialized_data = (char *)malloc(serialized_size);
if (serialized_data == NULL) {
    fprintf(stderr, "Erreur lors de l'allocation de mémoire\n");
    exit(EXIT_FAILURE);
}

int total_received = 0;
while (total_received < serialized_size) {
    int received = recv(clientSocket, serialized_data + total_received, serialized_size - total_received, 0);
    if (received == -1) {
        fprintf(stderr, "Erreur lors de la réception des données\n");
        free(serialized_data);
        exit(EXIT_FAILURE);
    }
    total_received += received;
}

// Désérialisation
//FileData *deserialized_files;
FileData *files;
int deserialized_count = deserialize_file_data(serialized_data, &files);
int nbre_ligne= deserialized_count;
free(serialized_data);
  //jof
     printf("%-5s  %-30s %s\n", "ID", "Nom du fichier", "Taille");
    for (int i = 0; i < nbre_ligne; ++i) {
        if(files[i].size >= 1024){
            if(files[i].size >= 1024*1024){
                 printf("%-5d  %-30s %d Mo\n", files[i].id, files[i].filename, (files[i].size)/(1024*1024));
             }else{
              printf("%-5d  %-30s %d Ko\n", files[i].id, files[i].filename, (files[i].size)/1024);
             }
        }else{
        printf("%-5d  %-30s %d o\n", files[i].id, files[i].filename, files[i].size);
        }
    }
   
    //choisir le fichier à télécharger 
    int no_file;
    char tmp[4];
    printf("++ Please, enter de file number to download:\t");
    scanf("%s",tmp);
        no_file = atoi(tmp);
    
   // no_file --;
    
    if(no_file <= nbre_ligne && no_file > 0){
        no_file --;
        char *nom_fichier= files[no_file].filename;
        char *ip_address = files[no_file].ip; 
        int file_size=files[no_file].size; 

        //printf("le telechargement du fichier %s à partir de   %s \n",nom_fichier,ip_address); 
       download_data_client(nom_fichier,ip_address,file_size);


    }else{
        printf("file not found\n");
    }
 

}



// long get_lenghtFile(char *nom_fichier){

//     struct stat st;

//     // Vérification si l'appel de stat a réussi
//     if (stat(filename, &st) == 0) {
//         // Vérification si la taille du fichier dépasse la capacité de long
//         if (st.st_size > LONG_MAX) {
//             fprintf(stderr, "Erreur : le fichier est trop grand pour être représenté par un long\n");
//             return -1;
//         }
//         return (long)st.st_size; // Taille du fichier en octets convertie en long
//     } else {
//         perror("stat"); // Affiche une erreur si stat échoue
//         return -1; // Retourne -1 en cas d'erreur
    
// }
// }

// fonction qui permet d'envoyer son debit de transmission au serveur



// Fonction pour désérialiser les données
int deserialize_file_data(char *serialized_data, FileData **files) {
    int file_count;
    memcpy(&file_count, serialized_data, sizeof(int));
    serialized_data += sizeof(int);

    *files = (FileData *)malloc(file_count * sizeof(FileData));
    if (*files == NULL) {
        fprintf(stderr, "Erreur lors de l'allocation de mémoire\n");
        return -1;
    }

    for (int i = 0; i < file_count; i++) {
        memcpy(&(*files)[i], serialized_data, sizeof(FileData));
        serialized_data += sizeof(FileData);
    }

    return file_count;
}




struct net_stat {
    char iface[IFNAMSIZ];
    unsigned long rx_bytes;
    unsigned long tx_bytes;
};

void get_net_stat(struct net_stat *stats, int *count) {
    FILE *fp;
    char line[256];
    int i = 0;

    fp = fopen("/proc/net/dev", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/net/dev");
        exit(EXIT_FAILURE);
    }

    // Skip the first two lines
    fgets(line, sizeof(line), fp);
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, " %[^:]: %lu %*d %*d %*d %*d %*d %*d %*d %lu", stats[i].iface, &stats[i].rx_bytes, &stats[i].tx_bytes);
        i++;
    }

    *count = i;
    fclose(fp);
}

unsigned long get_tx_rate(const char *interface) {
    struct net_stat prev_stats[10], curr_stats[10];
    int count, i;
    unsigned long tx_rate = 0;

    // Initial stats
    get_net_stat(prev_stats, &count);
    sleep(INTERVAL);

    // Stats after interval
    get_net_stat(curr_stats, &count);

    // Calculate TX rate for the specified interface
    for (i = 0; i < count; i++) {
        if (strcmp(prev_stats[i].iface, interface) == 0) {
            tx_rate = (curr_stats[i].tx_bytes - prev_stats[i].tx_bytes) / INTERVAL;
            break;
        }
    }

    return tx_rate;
}

