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

void writeLabel(FILE *file, char *label) {
    fprintf(file, "%s", label);
}

void writeLine(FILE *file, char *line) {
    fprintf(file, "%s\n", line);
}

FILE *createFile(char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("Error: Could not create file %s\n", path);
        exit(1);
    }
    return file;
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

