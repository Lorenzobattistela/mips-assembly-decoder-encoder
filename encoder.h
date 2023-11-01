#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "helpers.h"

#ifndef ENCODER_H
#define ENCODER_H

char *trim(char *line);

int getOpcodeFromAsm(char *instruction);

char **splitRTypeString(char *instruction);

char *encodeInstructionToBinary(char **splittedInstruction);

#endif