 #include "bittorent.h"



 

// /**
//  * Divise un fichier en 10 blocs de taille adaptée et calcule le hash SHA-256 de chaque bloc.
//  * 
//  * @param file_path Chemin d'accès au fichier
//  * @param blocks Tableau de structures de type file_block_t (alloué dynamiquement)
//  * @param num_blocks Nombre de blocs dans lesquels le fichier a été divisé (toujours 10)
//  * @return 0 en cas de succès, -1 en cas d'erreur
//  */
// int divide_file_into_blocks(const char *file_path, file_block_t **blocks, size_t *num_blocks) {
//     FILE *file = fopen(file_path, "rb");
//     if (file == NULL) {
//         return -1;
//     }

//     // Obtenir la taille du fichier
//     fseek(file, 0, SEEK_END);
//     size_t file_size = ftell(file);
//     fseek(file, 0, SEEK_SET);

//     *num_blocks = 10;
//     *blocks = malloc(*num_blocks * sizeof(file_block_t));

//     unsigned char block[file_size / *num_blocks + 1];
//     size_t read_size;
//     unsigned char hash[SHA256_DIGEST_LENGTH];
//     char hash_str[SHA256_DIGEST_LENGTH * 2 + 1];

//     for (size_t i = 0; i < *num_blocks; i++) {
//         read_size = fread(block, 1, sizeof(block), file);
//         SHA256(block, read_size, hash);
//         sprintf(hash_str, "%02x", hash[0]);
//         for (size_t j = 1; j < SHA256_DIGEST_LENGTH; j++) {
//             sprintf(hash_str + 2 * j, "%02x", hash[j]);
//         }

//         (*blocks)[i].block = malloc(read_size);
//         memcpy((*blocks)[i].block, block, read_size);
//         (*blocks)[i].hash = malloc(strlen(hash_str) + 1);
//         strcpy((*blocks)[i].hash, hash_str);
//         (*blocks)[i].size = read_size;
//     }

//     fclose(file);
//     printf(" block     hash    size\n");

//   for (size_t i = 0; i < num_blocks; i++) {
//     printf("Bloc %zu:\n", i);
//     printf("  Taille du bloc : %zu bytes\n", blocks[i].size);
//     printf("  Hash SHA-256 : %s\n", blocks[i].hash);
//     // Vous pouvez également afficher le contenu du bloc si nécessaire
//     // printf("  Contenu du bloc :\n");
//     // fwrite(blocks[i].block, 1, blocks[i].size, stdout);
//     // printf("\n");
// }
//     return 0;
// }