#include "encoder.h"

char *trim(char *line) {
    char *end;

    while(isspace((unsigned char)*line)) line++;

    if(*line == 0) return line;

    end = line + strlen(line) - 1;
    while(end > line && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

    return line;
}


char *encodeInstructionToBinary(char **splittedInstruction) {
    char *opcode = splittedInstruction[0];
    char *rd = splittedInstruction[1];
    char *rs = splittedInstruction[2];
    char *rt = splittedInstruction[3];

    int opcodeInt = getOpcodeFromAsm(opcode);
    int rdInt = getRegisterInt(rd);
    printf("rdInt: %d\n", rdInt);
    int rsInt = getRegisterInt(rs);
    printf("rsInt: %d\n", rsInt);
    int rtInt = getRegisterInt(rt);
    printf("rtInt: %d\n", rtInt);

    char *opcodeBinary = intToBinary(opcodeInt, 6);
    printf("opcodeBinary: %s\n", opcodeBinary);
    char *rdBinary = intToBinary(rdInt, 5);
    printf("rdBinary: %s\n", rdBinary);
    char *rsBinary = intToBinary(rsInt, 5);
    printf("rsBinary: %s\n", rsBinary);
    char *rtBinary = intToBinary(rtInt, 5);
    printf("rtBinary: %s\n", rtBinary);

    char *shamt = "00000";
    int funct = getFunctBinaryString(opcode);
    char *functBinary = intToBinary(funct, 6);

    char *binaryInstruction = malloc(32 * sizeof(char));
    
    sprintf(binaryInstruction, "%s%s%s%s%s%s", opcodeBinary, rsBinary, rtBinary, rdBinary, shamt, functBinary);
    printf("\n%ld\n", strlen(binaryInstruction));
    return binaryInstruction;
}


char **splitRTypeString(char *instruction) {
    // op, reg1, reg2, reg3
    char **splitted = malloc(4 * sizeof(char *));

    const char *delimiter = ", ";
    char *token = strtok(instruction, delimiter);
    int i = 0;

    while (token != NULL && i < 4) {
        splitted[i] = strdup(token);
        if (splitted[i] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(1);
        }
        i++;
        token = strtok(NULL, delimiter);
    }

    free(instruction); // Free the copied string
    return splitted;
}


int getOpcodeFromAsm(char *instruction) {
    // beq 1, 2, label
    if(strncmp(instruction, "j", 1) == 0) {
        return 2;
    }
    else if(strncmp(instruction, "sw", 2) == 0) {
        return 43;
    }
    else if(strncmp(instruction, "lw", 2) == 0) {
        return 23;
    }
    else if(strncmp(instruction, "beq", 3) == 0) {
        return 4;
    }
    else if(strncmp(instruction, "sltiu", 5) == 0) {
        return 11;
    }
    return 0;
}