#include <stdio.h>
#include <stdlib.h>
#include "io.h"

char *getNextLine(FILE *file) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    read = getline(&line, &len, file);
    if (read == -1) {
        return NULL;
    }
    return line;
}

void writeLine(FILE *file, char *line) {
    fprintf(file, "%s", line);
}

FILE *getFile(char *path, char *mode) {
    FILE *file = fopen(path, mode);
    if (file == NULL) {
        printf("Error: Could not open file %s\n", path);
        exit(1);
    }
    return file;
}

void closeFile(FILE *file) {
    fclose(file);
}

