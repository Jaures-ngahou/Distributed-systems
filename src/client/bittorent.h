#ifndef BITTORENT_H
#define BITTORENT_H
#include "handling_client.h"
#include <openssl/sha.h>

typedef struct {
    char *hash;
    size_t size;
} file_block_t;

int divide_file_into_blocks(const char *file_path, file_block_t **blocks, size_t *num_blocks) ;

#endif