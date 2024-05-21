#include "download_client.h"
// fonction qui permet de telecharger un fichier    partir d'un client
int download_data_client( char *nom_fichier,char *ip_address){
     int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
       fprintf(stderr,"socket");
       // exit(EXIT_FAILURE);
       return -1 ;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8090);
    server_addr.sin_addr.s_addr = inet_addr(ip_address);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
       // fprintf(stderr,"connect");
       printf("erreur lors du telechargement\n");
       return -1;
        //exit(EXIT_FAILURE);
    }

    if (send(sock, nom_fichier, strlen(nom_fichier), 0) == -1) {
        fprintf(stderr,"send");
       // exit(EXIT_FAILURE);
       return -1 ;
    }
    char buffer[512];
    int size;
    FILE *file1=fopen(nom_fichier,"w");

    while((size= recv(sock, buffer,sizeof(buffer),0)) > 0){
         if (strncmp(buffer, "EOF", 3) == 0) {
            break;
        }
        fwrite(buffer,1,size,file1);


    }
    if(file1 < 0){
      fprintf(stderr,"error");
    }
    printf(" download successfully!!\n");
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
    struct sockaddr_in client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    int client_sock = accept(server_socket,(struct sockaddr *)&client_addr, &client_addrlen);
    if (client_sock == -1){
        fprintf(stderr,"accept");
        return  -1 ;
    }
    char nom_fichier[100];
    while(1){
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
       // printf("data send to another client\n");




    }
   return 0;

}






