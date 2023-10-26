#ifndef IO_H
#define IO_H

#include <stdio.h>

FILE *getFile(char *path, char *mode);
char *getNextLine(FILE *file);
void closeFile(FILE *file);
void writeLine(FILE *file, char *line);
FILE *createFile(char *path);
void writeLabel(FILE *file, char *label);

#endif
