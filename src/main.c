#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include "aes.h"

#ifdef __GNUC__
#define likely(cond) __builtin_expect(!!(cond),1)
#define unlikely(cond) __builtin_expect(!!(cond),0)
#else
#define likely(cond) (!!(cond))
#define unlikely(cond) (!!(cond))
#endif

int encrypt(FILE *input, FILE *output, const unsigned int buffer_size) {
    unsigned int sz = buffer_size;
    uint8_t *data;
    struct AES_ctx ctx;

    /* FIXME: key is not random */
    uint8_t key[16] = {1};
    uint8_t iv [16] = {2};
    AES_init_ctx_iv(&ctx, key, iv);
    
    data = malloc(buffer_size);
    if (data == NULL) {
        if (buffer_size == 0)
            fputs("Error: buffer_size is 0\n", stderr);
        else
            fputs("Error: could not allocate memory\n", stderr);
        return 1;
    }
    while (likely(sz == buffer_size)) {
        sz = fread(data, sizeof(char), buffer_size, input);
        memset(&data[sz], 0, buffer_size - sz);
        AES_CBC_encrypt_buffer(&ctx, data, buffer_size);
        fwrite(data, sizeof(char), buffer_size, output);
    }
    memset(&data[sz], 0, buffer_size - sz);
    free(data);
    return 0;
}

int main(int argc, char **argv) {
    FILE *fpi, *fpo;
    int exit = 0;
    
    fpi = fopen("input.txt", "r");

    if (unlikely(fpi == NULL)) {
        perror("Error opening input file");
        return 1;
    }

    fpo = fopen("output.txt", "w");
    
    if (unlikely(fpo == NULL)) {
        perror("Error opening output file");
        return 1;
    }

    exit = encrypt(fpi, fpo, 1024);
    
    fclose(fpi);
    fclose(fpo);
    return exit;
}
