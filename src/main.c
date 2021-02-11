#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

#ifdef __GNUC__
#define likely(cond) __builtin_expect(!!(cond),1)
#define unlikely(cond) __builtin_expect(!!(cond),0)
#else
#define likely(cond) (!!(cond))
#define unlikely(cond) (!!(cond))
#endif

int encrypt(FILE *input, FILE *output, const unsigned int buffer_size) {
    unsigned int sz;
    uint8_t *data;

    data = malloc(buffer_size);
    if (data == NULL) {
        if (buffer_size == 0)
            fputs("Error: buffer_size is 0\n", stderr);
        else
            fputs("Error: could not allocate memory\n", stderr);
        return 1;
    }
    while (1) {
        sz = fread(data, sizeof(char), buffer_size, input);
        memset(&data[sz], 0, buffer_size - sz);
        if (fwrite(data, sizeof(char), buffer_size, output) != buffer_size) {
            perror("Error writing data");
            return 1;
        }
        if (sz != buffer_size) break;
    }
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

    exit = encrypt(fpi, fpo, 2);
    
    fclose(fpi);
    fclose(fpo);
    return exit;
}
