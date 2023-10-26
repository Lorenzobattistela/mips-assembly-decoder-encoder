#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#ifndef DECODER_H
#define DECODER_H


#define INITIAL_ADDRESS 0x400000

enum funct_opcodes {
    FUNCT_SUB = 0x22,
    FUNCT_OR = 0x25,
    FUNCT_AND = 0x24,
};

enum opcodes {
    OPCODE_R_TYPE = 0x0,
    OPCODE_SW = 0x2B,
    OPCODE_LW = 0x23,
    OPCODE_BEQ = 0x4,
    OPCODE_SLTIU = 0xB,
    OPCODE_J = 0x2,
};

enum registers {
    ZERO = 0,
    AT = 1,
    V0 = 2,
    V1 = 3,
    A0 = 4,
    A1 = 5,
    A2 = 6,
    A3 = 7,
    T0 = 8,
    T1 = 9,
    T2 = 10,
    T3 = 11,
    T4 = 12,
    T5 = 13,
    T6 = 14,
    T7 = 15,
    S0 = 16,
    S1 = 17,
    S2 = 18,
    S3 = 19,
    S4 = 20,
    S5 = 21,
    S6 = 22,
    S7 = 23,
    T8 = 24,
    T9 = 25,
    K0 = 26,
    K1 = 27,
    GP = 28,
    SP = 29,
    FP = 30,
    RA = 31,
};

bool isLabel(char *string);

int getLabelAddress(int labelInstructionCount);

char *getRegisterString(int register);

char *getOperationWithFunctString(int funct);

char *getOperationString(int opcode);

char *mountTypeJInstructionString(char **splittedInstruction);

char *mountTypeRInstructionString(char **splittedInstruction);

char *mountTypeIInstructionString(char **splittedInstruction);

int binaryStringToInt(char *binaryStr);

char *hexToBinary(const char *hexValue);

int hexStringToInt(char *hexString);

char **splitITypeInstruction(char *binaryInstruction);

char **splitRTypeInstruction(char *binaryInstruction);

int getOpcode(char *binaryString);

char *createLabel();

bool isBeqInstruction(char *instruction);

int getBeqInstructionDesloc(char *instruction);

char *getLabelFromInstruction(char *instruction);


#endif