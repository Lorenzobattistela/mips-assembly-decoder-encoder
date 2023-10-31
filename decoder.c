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

    char *jumpAddr = fixJumpAddress(splittedInstruction[1]);

    int immediate = binaryStringToInt(jumpAddr);

    sprintf(instructionString, "%s %d", operationString, immediate);

    return instructionString;
}

int getAddressFromJInstruction(char *instruction) {
    // get all chars except for the 2 first (j )
    char *address = instruction + 2;
    return atoi(address);
}

char *fixJumpAddress(char *jump) {
    printf("jump: %s\n", jump);
    char *newJump = malloc(100);
    // add 4 0 bits in the start of the string and 2 0 bits at the end
    sprintf(newJump, "0000%s00", jump);
    return newJump;
}

char *shortJumpAddress(char *jump) {
    char *newJump = malloc(100);
    strncpy(newJump, jump + 4, 26); // remove first 4 bits and copy just 26 (removing 2 last);
    newJump[26] = '\0';
    return newJump;
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

        int immediate = binaryStringToIntWithNegatives(splittedInstruction[3]);
        printf("IMMEDIATE: %d\n", immediate);

        sprintf(instructionString, "%s %s, %d(%s)", operationString, registerString2, immediate, registerString1);
        return instructionString;
    }

    else if(strncmp(operationString, "beq", 3) == 0) {
        char *instructionString = malloc(100);

        char *registerString1 = getRegisterString(binaryStringToInt(splittedInstruction[1]));
        char *registerString2 = getRegisterString(binaryStringToInt(splittedInstruction[2]));
        int desloc = binaryStringToIntWithNegatives(splittedInstruction[3]);
        printf("Binary string to int with negatives: %d\n", binaryStringToIntWithNegatives(splittedInstruction[3]));
        char *label = createLabel();

        sprintf(instructionString, "step:%d; %s %s, %s, %s", desloc, operationString, registerString1, registerString2, label);

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

char **splitJTypeInstruction(char *binaryInstruction) {
    if (strlen(binaryInstruction) != 32) {
        return NULL;
    }
    char **res = malloc(2 * sizeof(char *));
    res[0] = malloc(7);
    res[1] = malloc(26);

    strncpy(res[0], binaryInstruction, 6);
    res[0][6] = '\0';

    strncpy(res[1], binaryInstruction + 6, 26);
    res[1][26] = '\0';

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

int counter = 0;
char *createLabel() {
    char *label = malloc(100);
    sprintf(label, "label_%d", counter);
    counter++;
    return label;
}

char *getLabel(int i) {
    char *label = malloc(10);
    sprintf(label, "label_%d:\t", i);
    return label;
}

bool isBeqInstruction(char *instruction) {
    if(strncmp(instruction, "step:", 5) == 0) {
        return true;
    }
    return false;
}

char *removeBeqPrefix(char *instruction) {
    char *newInstruction = instruction + 8;
    return newInstruction;
}

int getBeqInstructionDesloc(char *instruction) {
    // remove step: from str
    char *instructionWithoutStep = instruction + 5;
    char *desloc = malloc(100);
    for(int i = 0; i < strlen(instructionWithoutStep); i++) {
        if(instructionWithoutStep[i] == ';') {
            // copy the desloc (str[0:i])
            strncpy(desloc, instructionWithoutStep, i);
            desloc[i] = '\0';
            instruction + 1;
            return atoi(desloc);
        }
    }
}

char *getLabelFromInstruction(char *instruction) {
    char *label = malloc(10);
    char *instructionCopy = strdup(instruction);
    // label_n -> get last 7 chars of the str
    strncpy(label, instructionCopy + strlen(instructionCopy) - 7, 7);
    return label;  
}

int calculateAddress(int currentAddress, int desloc) {
    return currentAddress + (desloc * 4);
}

int calculateIndexFromAddress(int address) {
    return (address - INITIAL_ADDRESS) / 4;
}

int binaryStringToIntWithNegatives(char *binaryStr) {
    int length = strlen(binaryStr);
    int isNegative = 0;

    if (length > 0 && binaryStr[0] == '1') {
        isNegative = 1;
    }

    long number = strtol(binaryStr, NULL, 2);

    if (isNegative) {
        number -= (1 << length); // Subtract 2^length
    }
    return (int)number;
}