#include "encoder.h"

char *trim(char *line) {
    char *end;

    while(isspace((unsigned char)*line)) line++;

    if(*line == 0) return "";

    end = line + strlen(line) - 1;
    while(end > line && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

    return line;
}


char *encodeRInstructionToBinary(char **splittedInstruction) {
    char *opcode = splittedInstruction[0];
    char *rd = splittedInstruction[1];
    char *rs = splittedInstruction[2];
    char *rt = splittedInstruction[3];

    int opcodeInt = getOpcodeFromAsm(opcode);
    int rdInt = getRegisterInt(rd);
    int rsInt = getRegisterInt(rs);
    int rtInt = getRegisterInt(rt);

    char *opcodeBinary = intToBinary(opcodeInt, 6);
    char *rdBinary = intToBinary(rdInt, 5);
    char *rsBinary = intToBinary(rsInt, 5);
    char *rtBinary = intToBinary(rtInt, 5);

    char *shamt = "00000";
    int funct = getFunctBinaryString(opcode);
    char *functBinary = intToBinary(funct, 6);

    char *binaryInstruction = malloc(32 * sizeof(char));
    
    sprintf(binaryInstruction, "%s%s%s%s%s%s", opcodeBinary, rsBinary, rtBinary, rdBinary, shamt, functBinary);
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
    return splitted;
}

char **splitITypeString(char *instruction) {
    // op, reg1, reg2, imm
    char **splitted = malloc(4 * sizeof(char *));

    // check if is SW or LW
    if(strncmp(instruction, "lw", 2) == 0 || strncmp(instruction, "sw", 2) == 0) {
        // sw reg1, imm(reg2)
        // lw reg1, imm(reg2)
        // split correctly

        const char* delimiter = ", ";
        char *token = strtok(instruction, delimiter);
        int i = 0;
        while (token != NULL && i < 2) {
            splitted[i] = strdup(token);
            if (splitted[i] == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                exit(1);
            }
            i++;
            token = strtok(NULL, delimiter);
        }

        // here we get splited[0] = opcode
        // and splited[1] = reg1
        // now we need to get imm and reg2

        size_t removedStrLength = strlen(splitted[0]) + strlen(splitted[1]) + 3;
        char *ins = instruction + removedStrLength;

        int j = 0;
        char *imm = malloc(10 * sizeof(char));
        while(ins[j] != '(' && i < strlen(ins)) {
            imm[j] = ins[j];
            j++;
        }
        imm[j] = '\0';

        splitted[3] = strdup(imm);

        j++;
        int k = 0;
        char *reg2 = malloc(10 * sizeof(char));
        while(ins[j] != ')' && i < strlen(ins)) {
            reg2[k] = ins[j];
            j++;
            k++;
        }
        reg2[k] = '\0';

        splitted[2] = strdup(reg2);

        // swap splitted[1] and splitted[2]
        char *temp = splitted[1];
        splitted[1] = splitted[2];
        splitted[2] = temp;

        return splitted;
    }

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
    return splitted;
}

char *encodeIInstructionToBinary(char **splitted) {
    char *opcode = splitted[0];
    char *rs = splitted[1];
    char *rt = splitted[2];
    char *imm = splitted[3];

    int opcodeInt = getOpcodeFromAsm(opcode);
    int rtInt = getRegisterInt(rt);
    int rsInt = getRegisterInt(rs);
    int immInt = atoi(imm);
  
    char *opcodeBinary = intToBinary(opcodeInt, 6);
    char *rtBinary = intToBinary(rtInt, 5);
    char *rsBinary = intToBinary(rsInt, 5);
    char *immBinary = intToBinary(immInt, 16);

    char *binaryInstruction = malloc(32 * sizeof(char));
    sprintf(binaryInstruction, "%s%s%s%s", opcodeBinary, rsBinary, rtBinary, immBinary);
    return binaryInstruction;
}


char **splitJTypeString(char *instruction) {
    if (instruction == NULL) {
        fprintf(stderr, "Input instruction is NULL.\n");
        return NULL;
    }
    
    char **splitted = malloc(2 * sizeof(char *));
    if (splitted == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }

    const char *delimiter = " ";
    char *token = strtok(instruction, delimiter);
    int i = 0;

    while (token != NULL && i < 2) {
        splitted[i] = strdup(token);
        if (splitted[i] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(1);
        }
        i++;
        token = strtok(NULL, delimiter);
    }

    return splitted;
}

char *encodeJInstructionToBinary(char **splittedInstruction, FILE *inputFile) {
    char *opcode = splittedInstruction[0];
    char *label = splittedInstruction[1];

    int opcodeInt = getOpcodeFromAsm(opcode);
    int labelAddress = getLabelAddressFromFile(inputFile, label);

    char *opcodeBinary = intToBinary(opcodeInt, 6);
    char *addressBinary = intToBinary(labelAddress, 26);

    char *binaryInstruction = malloc(32 * sizeof(char));
    sprintf(binaryInstruction, "%s%s", opcodeBinary, addressBinary);
    return binaryInstruction;
}

int getLabelAddressFromFile(FILE *f, char *label) {
    int initialAddress = 0x00400000;
    size_t labelLength = strlen(label);

    // skip first 3 lines
    for(int i = 0; i < 3; i++) {
        char *line = getNextLine(f);
        free(line);
    }

    char *line;
    while ((line = getNextLine(f)) != NULL) {
        char *trimmedLine = trim(line);
        if(strncmp(trimmedLine, label, labelLength) == 0) {
            return initialAddress;
        }
        initialAddress += 4;
    }
    return -1;
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
        return 35;
    }
    else if(strncmp(instruction, "beq", 3) == 0) {
        return 4;
    }
    else if(strncmp(instruction, "sltiu", 5) == 0) {
        return 11;
    }
    return 0;
}
