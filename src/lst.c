#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

struct KeyValue {
    char *key;
    int value;
};
typedef struct KeyValue *
int main() {

    struct KeyValue myMap[100]; // Tableau de paires clé-valeur
    int mapSize = 0; // nombre actuel d'elements dans le tableau
    DIR *dir;
    struct dirent *entry;
    struct stat file_info;

    dir = opendir("./data/");
    if (dir == NULL) {
        perror("Erreur lors de l'ouverture du dossier");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        char file_path[300];
        snprintf(file_path, sizeof(file_path), "./data/%s", entry->d_name);

        if (stat(file_path, &file_info) == 0) { // obtenier des infos sur les autres fichiers
            if (S_ISREG(file_info.st_mode)) { // verifier si c'est un fichier simple et non un lien sym
                printf("Fichier : %s, Taille : %ld octets\n", entry->d_name, file_info.st_size);

                 myMap[mapSize].key= entry->d_name;
                 myMap[mapSize].value = file_info.st_size;
                 mapSize ++;
            }
        }
    }
    printf("cles valeur \n");        
    closedir(dir);
     for (int i = 0; i < mapSize; i++) 
           printf("Valeur pour la clé '%s': %d\n",myMap[i].key, myMap[i].value);
   
    return 0;
}



// int main() {
//     struct KeyValue myMap[100]; // Tableau de paires clé-valeur
//     int mapSize = 0; // nombre actuel d'elements dans le tableau

//     // Ajout d'une paire clé-valeur
//     myMap[mapSize].key= "age";
//     //strcpy(myMap[mapSize].key, "age");
//     myMap[mapSize].value = 30;
//     mapSize++;
//     myMap[mapSize].key= "fi1.txt";
//     myMap[mapSize].value = 40;
//     mapSize++;


//     // Récupération de la valeur associée à la clé "age"
//     for (int i = 0; i < mapSize; i++) {
//     //    // if (strcmp(myMap[i].key, "age") == 0) { // positif si s1>s2
//            printf("Valeur pour la clé '%s': %d\n",myMap[i].key, myMap[i].value);
//     //         break;
//     //     }
//     }

//     return 0;
// }
