#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "helpers.h"
#include "io.h"

#ifndef ENCODER_H
#define ENCODER_H

char *trim(char *line);

int getOpcodeFromAsm(char *instruction);

char **splitRTypeString(char *instruction);

char *encodeRInstructionToBinary(char **splittedInstruction);

char **splitJTypeString(char *instruction);

char *encodeJInstructionToBinary(char **splittedInstruction, FILE *inputFile);

int getLabelAddressFromFile(FILE *f, char *label);

char **splitITypeString(char *instruction);

char *encodeIInstructionToBinary(char **splitted);

#endif