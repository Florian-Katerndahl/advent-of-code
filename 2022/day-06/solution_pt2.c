#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

char * read_data_stream(char *dst, FILE *f);

size_t find_offset(char *src);

int main(int argc, char *argv[]) {
    assert(argc == 2);

    FILE *f = fopen(argv[1], "rt");

    char *data_stream = malloc(sizeof(char));

    if (f == NULL) {
        fprintf(stderr, "Error, failed to open file %s\n", argv[1]);
        exit(1);
    }

    data_stream = read_data_stream(data_stream, f);

    printf("Message starts at offset %ld\n", find_offset(data_stream) + 1UL);

    fclose(f);
    free(data_stream);

    return 0;
}

char * read_data_stream(char *dst, FILE *f) {
    int c;
    size_t buff_size = 0, current_size = 0; // not entirely true, but whatever....

    while ((c = fgetc(f)) != '\n' && c != EOF) {
        if (current_size + 1 > buff_size) {
            dst = realloc(dst, current_size + 100);
            buff_size += 100;
        }
        dst[current_size] = (char) c;
        current_size++;
    }

    if (current_size + 1 > buff_size) {
        dst = realloc(dst, current_size + 1);
    }

    dst[current_size] = '\0';

    return dst;
}

size_t find_offset(char *src) {
    if (strlen(src) < 14)
        return 0;

    char *offset_pointer = src + 13;
    char char_map[127];
    int sum;

    while (*offset_pointer) {
        sum = 0;
        memset(char_map, 0, 127);

        for (size_t i = 0; i < 14; i++)
            char_map[(int) *(offset_pointer - i)]++;

        for (size_t i = 0; i < 128; i++)
            if (char_map[i] == 1)
                sum++;

        if (sum == 14)
            return (size_t) (offset_pointer - src);

        offset_pointer++;
    }

    return 0;
}
