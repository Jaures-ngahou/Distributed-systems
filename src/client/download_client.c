#include "download_client.h"

void print_progress_bar(int percentage) {
    int bar_width = 50;  // Largeur de la barre de progression
    int pos = (percentage * bar_width) / 100;

    printf("[");
    for (int i = 0; i < bar_width; ++i) {
        if (i < pos) printf("=");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %d%%\r", percentage);
    fflush(stdout);  // Force l'affichage immédiat
}


void showProgressBar(int current, int total) {
    int barWidth = 70;
    float progress = (float)current / total;

    printf("\r[");
    int pos = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) printf("=");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %d%%  ", (int)(progress * 100.0));
    fflush(stdout);
}

int download_data_client(char *nom_fichier, char *ip_address, int file_size1) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8090);
    server_addr.sin_addr.s_addr = inet_addr(ip_address);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sock);
        return -1;
    }

    if (send(sock, nom_fichier, strlen(nom_fichier), 0) == -1) {
        perror("send");
        close(sock);
        return -1;
    }

    char buffer[512];
    int size;
    FILE *file1 = fopen(nom_fichier, "w");
    if (file1 == NULL) {
        perror("fopen");
        close(sock);
        return -1;
    }

      int total_received = 0;
    printf("Downloading file '%s' (%d bytes):\n", nom_fichier, file_size1);

    while ((size = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        // Vérification du message de fin "EOF" seulement si c'est la seule chose reçue dans le buffer
        if ( strncmp(buffer, "EOF", 3) == 0) {
            break;
        }
        fwrite(buffer, 1, size, file1);
        total_received += size;

        // Calculez correctement le pourcentage, en évitant les valeurs négatives ou au-delà de 100%
        // int percentage = (total_received * 100) / file_size1;
        // if (percentage < 0) {
        //    percentage = 0;
        // } else if (percentage > 100) {
        //     percentage = 100;
        // }
        // print_progress_bar(percentage);
     showProgressBar(total_received,file_size1) ;

    }

    if (size == -1) {
        perror("recv");
        fclose(file1);
        close(sock);
        return -1;
    }

    printf("\nDownload successfully!!\n");
    fclose(file1);
    close(sock);

    return 0;
}





//fonction qui permet d'envoyer les fichiers    un client 
int  download_data_server(int port){
    int server_socket = socket(AF_INET,SOCK_STREAM,0);
    if(server_socket == -1){
        fprintf(stderr,"socket");
        return  -1 ;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_socket,(struct sockaddr *)&server_addr,sizeof(server_addr))== -1){
        fprintf(stderr,"bind");
        return  -1 ;
    }
    if(listen( server_socket,10) == -1){
        fprintf(stderr,"listen");
        return  -1 ;

    }
    printf("connected\n");

    //client
    while(1){
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = sizeof(client_addr);
        int client_sock = accept(server_socket,(struct sockaddr *)&client_addr, &client_addrlen);
        if (client_sock == -1){
            fprintf(stderr,"accept");
            return  -1 ;
        }
            char nom_fichier[100];
        
                ssize_t bytesReceived = recv(client_sock, nom_fichier, sizeof(nom_fichier) - 1, 0);
            if (bytesReceived == -1) {
            fprintf(stderr,"recv");
            exit(EXIT_FAILURE);
            }
            nom_fichier[bytesReceived] = '\0'; // Ajouter un caract  re nul de fin de cha  ne

            char message[115];
            sprintf(message,"./data/%s",nom_fichier);
        // printf("nom fichier %s",nom_fichier);
            FILE *fd= fopen(message,"r");
            if(!fd){
                fprintf(stderr,"file not found \n");
                return  -1 ;
            }

            char buffer[512];
            size_t num_read;
            do {
                num_read =fread(buffer,1,sizeof(buffer),fd);
                if (num_read < 0) {
                        printf("error in fread()\n");
                }

                if(send(client_sock,buffer,sizeof(buffer),0) == -1){
                    fprintf(stderr,"send");
                }
            } while (num_read > 0);
            char *end_of_file= "EOF";
        if( send(client_sock, end_of_file, sizeof(end_of_file),0) == -1){
            fprintf(stderr,"error_send");
        }
         printf("files sent to another client\n");




    }
   return 0;

}






