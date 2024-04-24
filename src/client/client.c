#include "handling_client.h"



int main(int argc, char **argv) {
    if(argc < 2){
        printf("syntaxe: ./client ip\n");
        exit(-1);
    }
    int option;
    char *ip_serveur= argv[1];
    printf("++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("++ Welcome to our file sharing application  ++\n");
    printf("++++++++++++++++++++++++++++++++++++++++++++++\n\n");
    printf("To display the list of file stored on the server, press 1\n");
    printf("To upload your files contained in \" data\" folder, press 2\n");
    printf("To update your files on the server, press 3\n");
    printf("To download files from server, press 4\n ");
    int i=0;
    while(i<5){

        scanf("%d",&option);
        switch(option){
            case 1:
                printf("sorry, it's available in prenium version\n");
            break;
            case 2:
                handle_event(ip_serveur);
            break;
            case 3:
                handle_event(ip_serveur);
                printf("data update \n");
            break;
            case 4:
                printf("sorry, it's available in prenium version\n");
            break;
            default:
            printf("Please, enter a valid number\n");
            break;
        }
        if(i!=4)
        printf("\n what do you want else:");
        i++;
    }

          

    


    return 0;
}

