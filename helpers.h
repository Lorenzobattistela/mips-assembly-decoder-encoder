#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef HELPERS_H
#define HELPERS_H

char *binaryToHexadecimal(char *binaryInstruction);

char *intToBinary(int n, int bits);

int getRegisterInt(char *regist);

int getFunctBinaryString(char *opcode);

#endif