#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef ENCODER_H
#define ENCODER_H

char *trim(char *line);

int getOpcodeFromAsm(char *instruction);

int getRegisterInt(char *register);

#endif