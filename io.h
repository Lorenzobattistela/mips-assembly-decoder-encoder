#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <string.h>

FILE *getFile(char *path, char *mode);
char *getNextLine(FILE *file);
void closeFile(FILE *file);
void writeLine(FILE *file, char *line);
FILE *createFile(char *path);
void writeLabel(FILE *file, char *label);
void writeLineWithoutBreak(FILE *file, char *line);
void deleteFile(char *path);

#endif
