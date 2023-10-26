// sub $t1, $s3, $s4
#include "decoder.h"


bool isLabel(char *string) {
    return string[strlen(string) - 1] == ':';
}

int getLabelAddress(int labelInstructionCount) {
    return INITIAL_ADDRESS + (labelInstructionCount * 4);
}

char *getRegisterString(int reg) {
    switch(reg) {
        case T0:
            return "$t0";
        case T1:
            return "$t1";
        case T2:
            return "$t2";
        case T3:
            return "$t3";
        case T4:
            return "$t4";
        case T5:
            return "$t5";
        case T6:
            return "$t6";
        case T7:
            return "$t7";
        case S0:
            return "$s0";
        case S1:
            return "$s1";
        case S2:
            return "$s2";
        case S3:
            return "$s3";
        case S4:
            return "$s4";
        case S5:
            return "$s5";
        case S6:
            return "$s6";
        case S7:
            return "$s7";
        case T8:
            return "$t8";
        case T9:
            return "$t9";
        case K0:
            return "$k0";
        case K1:
            return "$k1";
        case GP:
            return "$gp";
        case SP:
            return "$sp";
        case FP:
            return "$fp";
        case RA:
            return "$ra";
        case ZERO:
            return "$zero";
        case AT:
            return "$at";
        case V0:
            return "$v0";
        case V1:
            return "$v1";
        case A0:
            return "$a0";
        case A1:
            return "$a1";
        case A2:
            return "$a2";
        case A3:
            return "$a3";
        default:
            return NULL;
    }
}


char *getOperationWithFunctString(int funct) {
    printf("funct: %d\n", funct);
    switch(funct) {
        case FUNCT_SUB:
            return "sub";

        case FUNCT_OR:
            return "or";

        case FUNCT_AND:
            return "and";

        default:
            return NULL;
    }
}


char *getOperationString(int opcode) {
   switch(opcode) {
        case OPCODE_R_TYPE:
            return NULL;

        case OPCODE_SW:
            return "sw";
        
        case OPCODE_LW:
            return "lw";

        case OPCODE_BEQ:
            return "beq";
        
        case OPCODE_SLTIU:
            return "sltiu";
        
        case OPCODE_J:
            return "j";

        default:
           return NULL;
   }
}

char *mountTypeJInstructionString(char **splittedInstruction) {
    char *instructionString = malloc(100);
    char *operationString = getOperationString(binaryStringToInt(splittedInstruction[0]));
    int immediate = binaryStringToInt(splittedInstruction[1]);

    sprintf(instructionString, "%s %d", operationString, immediate);

    return instructionString;
}

char *mountTypeRInstructionString(char **splittedInstruction) {
    char *instructionString = malloc(100);
    char *operationString = getOperationWithFunctString(binaryStringToInt(splittedInstruction[5]));
    char *registerString1 = getRegisterString(binaryStringToInt(splittedInstruction[1]));
    char *registerString2 = getRegisterString(binaryStringToInt(splittedInstruction[2]));
    char *registerString3 = getRegisterString(binaryStringToInt(splittedInstruction[3]));

    sprintf(instructionString, "%s %s, %s, %s", operationString, registerString3, registerString1, registerString2);

    return instructionString;
}

char *mountTypeIInstructionString(char **splittedInstruction) {
    char *instructionString = malloc(100);
    char *operationString = getOperationString(binaryStringToInt(splittedInstruction[0]));

    if(strncmp(operationString, "sw", 2) == 0 || strncmp(operationString, "lw", 2) == 0) {
        char *registerString1 = getRegisterString(binaryStringToInt(splittedInstruction[1]));
        char *registerString2 = getRegisterString(binaryStringToInt(splittedInstruction[2]));

        int immediate = binaryStringToInt(splittedInstruction[3]);

        sprintf(instructionString, "%s %s, %d(%s)", operationString, registerString2, immediate, registerString1);
        return instructionString;
    }

    char *registerString1 = getRegisterString(binaryStringToInt(splittedInstruction[1]));
    char *registerString2 = getRegisterString(binaryStringToInt(splittedInstruction[2]));
    int immediate = binaryStringToInt(splittedInstruction[3]);

    sprintf(instructionString, "%s %s, %s, %d", operationString, registerString2, registerString1, immediate);

    return instructionString;
}


int binaryStringToInt(char *binaryStr) {
    char *endptr;
    long number = strtol(binaryStr, &endptr, 2);

    if (*endptr != '\0' && *endptr != '\n') {
        printf("Conversion error: Invalid characters in the binary string.\n");
        return -1;
    }

    return (int) number;
}



char *hexToBinary(const char *hexValue) {
    if (strncmp(hexValue, "0x", 2) == 0) {
        hexValue += 2;
    }

    long decimalValue = strtol(hexValue, NULL, 16);
    char binaryString[33];

    for (int bit = 31; bit >= 0; bit--) {
        int bitValue = (decimalValue >> bit) & 1;
        binaryString[31 - bit] = bitValue + '0';
    }
    binaryString[32] = '\0';
    return strdup(binaryString);
}

int hexStringToInt(char *hexString) {
    if (strncmp(hexString, "0x", 2) == 0) {
        hexString += 2;
    }
    int result;
    sscanf(hexString, "%x", &result);
    return result;
}

char **splitITypeInstruction(char *binaryInstruction) {
    if (strlen(binaryInstruction) != 32) {
        return NULL;
    }

    char **res = malloc(4 * sizeof(char *));
    res[0] = malloc(7);
    res[1] = malloc(6);
    res[2] = malloc(6);
    res[3] = malloc(17);

    strncpy(res[0], binaryInstruction, 6);
    res[0][6] = '\0';
    strncpy(res[1], binaryInstruction + 6, 5);
    res[1][5] = '\0';
    strncpy(res[2], binaryInstruction + 11, 5);
    res[2][5] = '\0';
    strncpy(res[3], binaryInstruction + 16, 16);
    res[3][16] = '\0';

    return res;
}

char **splitRTypeInstruction(char *binaryInstruction) {
    if (strlen(binaryInstruction) != 32) {
        return NULL;
    }

    char **res = malloc(6 * sizeof(char *));

    res[0] = malloc(7);
    res[1] = malloc(6);
    res[2] = malloc(6);
    res[3] = malloc(6);
    res[4] = malloc(6);
    res[5] = malloc(7);

    strncpy(res[0], binaryInstruction, 6);
    res[0][6] = '\0';
    strncpy(res[1], binaryInstruction + 6, 5);
    res[1][5] = '\0';
    strncpy(res[2], binaryInstruction + 11, 5);
    res[2][5] = '\0';
    strncpy(res[3], binaryInstruction + 16, 5);
    res[3][5] = '\0';
    strncpy(res[4], binaryInstruction + 21, 5);
    res[4][5] = '\0';
    strncpy(res[5], binaryInstruction + 26, 6);
    res[5][6] = '\0';

    return res;
}

int getOpcode(char *binaryString) {
    char *opcodeString = malloc(7);
    strncpy(opcodeString, binaryString, 6);
    opcodeString[6] = '\0';
    int opcode = binaryStringToInt(opcodeString);
    free(opcodeString);
    return opcode;
}